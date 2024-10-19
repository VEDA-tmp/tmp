#include "tvm_wrapper.h"
#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <chrono>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "tracker.h"
#include "utils.h"


// DetectedObject structure
struct DetectedObject {
    int id;
    cv::Rect boxes;
    cv::KalmanFilter kalman;
    float confidences;
    int class_ids;
    bool matched = false;
    int lost_frames = 0;  // To handle occlusion and disappearance
};

tvm::runtime::NDArray TVMWrapper::preprocess_frame(cv::Mat& frame,int _batch, int _input_w, int _input_h) {
    cv::Mat resized_img;
    cv::resize(frame, resized_img, cv::Size(640, 640));
    resized_img.convertTo(resized_img, CV_32F, 1.0 / 255.0);

    int batch = _batch;
    int height = _input_h;
    int width = _input_w;
    int channels = 3;

    std::vector<int64_t> shape = {batch, channels, height, width};
    tvm::runtime::NDArray input = tvm::runtime::NDArray::Empty(shape, DLDataType{kDLFloat, 32, 1}, DLDevice{kDLCPU, 0});

    // OpenCV의 Mat는 HWC 형식 (height, width, channels)이고 NDArray는 NCHW 형식이므로 데이터를 변환하면서 바로 복사하도록 했습니다.
    float* input_data = static_cast<float*>(input->data);

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            cv::Vec3f pixel = resized_img.at<cv::Vec3f>(h, w);
            input_data[0 * height * width + h * width + w] = pixel[0];  // R 채널
            input_data[1 * height * width + h * width + w] = pixel[1];  // G 채널
            input_data[2 * height * width + h * width + w] = pixel[2];  // B 채널
        }
    }

    return input;  // tvm::runtime::NDArray 반환
}

// IoU(Intersection over Union) 계산 함수
float IoU(const cv::Rect& box1, const cv::Rect& box2) {
    int x1 = std::max(box1.x, box2.x);
    int y1 = std::max(box1.y, box2.y);
    int x2 = std::min(box1.x + box1.width, box2.x + box2.width);
    int y2 = std::min(box1.y + box1.height, box2.y + box2.height);

    int interArea = std::max(0, x2 - x1) * std::max(0, y2 - y1);
    int box1Area = box1.width * box1.height;
    int box2Area = box2.width * box2.height;

    return static_cast<float>(interArea) / (box1Area + box2Area - interArea);
}

// Non-Maximum Suppression (NMS) 함수
std::vector<int> NonMaximumSuppression(const std::vector<DetectedObject>& tracking_object_group, float iou_threshold) {
    std::vector<int> indices;
    std::vector<int> sorted_indices(tracking_object_group.size());

    // 각 tracking_object의 cv::Rect 값을 추출하여 boxes 벡터에 추가
    std::vector<cv::Rect> boxes;
    std::vector<float> confidences;
    for (const auto& obj : tracking_object_group) {
        boxes.push_back(obj.boxes);  // 올바른 멤버 접근
        confidences.push_back(obj.confidences);
    }

    // 신뢰도를 기준으로 내림차순 정렬
    for (int i = 0; i < boxes.size(); ++i) {
        sorted_indices[i] = i;
    }
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int i, int j) {
        return confidences[i] > confidences[j];
    });

    std::vector<bool> suppressed(boxes.size(), false);
    for (int i = 0; i < sorted_indices.size(); ++i) {
        int idx = sorted_indices[i];
        if (suppressed[idx]) continue;

        indices.push_back(idx);

        for (int j = i + 1; j < sorted_indices.size(); ++j) {
            int next_idx = sorted_indices[j];
            if (suppressed[next_idx]) continue;

            // IoU 계산 후 겹치면 suppress
            if (IoU(boxes[idx], boxes[next_idx]) > iou_threshold) {
                suppressed[next_idx] = true;
            }
        }
    }
    return indices;
}


std::vector<cv::Rect> ProcessYOLOOutput(tvm::runtime::NDArray output, const std::vector<std::string>& class_names, cv::Mat& frame, 
                       std::vector<DetectedObject>& trackedObjects, int& currentID, float conf_threshold = 0.75) {

    
    LOG(INFO) << "detection start...";

    const int64_t* shape = output.Shape().data();
    int num_detections = shape[1];  // 감지된 객체 수

    float* output_data = static_cast<float*>(output->data);
    int num_classes = class_names.size();  // 클래스 개수

    int original_width = frame.cols;
    int original_height = frame.rows;
    const float width_ratio = static_cast<float>(original_width) / 640.0f;
    const float height_ratio = static_cast<float>(original_height) / 640.0f;

    const int data_stride = 5 + num_classes;

    std::vector<DetectedObject> detected_objects;
    

    // Extract detected objects
    for (int i = 0; i < num_detections; ++i) {
        int base_index = i * data_stride;

        float cx = output_data[base_index + 0] * width_ratio;
        float cy = output_data[base_index + 1] * height_ratio;
        float w = output_data[base_index + 2] * width_ratio;
        float h = output_data[base_index + 3] * height_ratio;
        float confidence = output_data[base_index + 4];

        if (confidence > conf_threshold) {
            float* class_scores = &output_data[base_index + 5];
            int class_id = std::distance(class_scores, std::max_element(class_scores, class_scores + num_classes));

            if (class_id == 0) {  // Only process "person" class
                int x1 = static_cast<int>(cx - (w / 2));
                int y1 = static_cast<int>(cy - (h / 2));
                int x2 = static_cast<int>(cx + (w / 2));
                int y2 = static_cast<int>(cy + (h / 2));

                DetectedObject obj;
                obj.id = -1;  // Will be assigned later
                obj.boxes = cv::Rect(x1, y1, x2 - x1, y2 - y1);
                obj.confidences = confidence;
                obj.class_ids = class_id;
                detected_objects.push_back(obj);
            }
        }
    }
    
    float iou_threshold = 0.4f; 
    std::vector<int> nms_indices = NonMaximumSuppression(detected_objects, iou_threshold);
    std::vector<cv::Rect> bbox_per_frame;

    // Display the tracked objects
    for (int idx : nms_indices) {
        bbox_per_frame.push_back(detected_objects[idx].boxes);
        // cv::rectangle(frame, detected_objects[idx].boxes, cv::Scalar(0, 255, 0), 2);
        // cv::putText(frame, "ID: " + std::to_string(detected_objects[idx].id), cv::Point(detected_objects[idx].boxes.x, detected_objects[idx].boxes.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }

    return bbox_per_frame;
}

// TVM 모델을 이용한 실시간 객체 탐지
void TVMWrapper::detect() {
    LOG(INFO) << this->model_name + "Model Setup...";
    tvm::runtime::PackedFunc set_input = mod.GetFunction("set_input");
    tvm::runtime::PackedFunc run = mod.GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod.GetFunction("get_output");

    std::vector<DetectedObject> trackedObjects; // To store all tracked objects
    int currentID = 0;

    cv::VideoCapture cap("비디오 파일 경로");
    cv::Mat frame;
    int frame_index = 0;

    Tracker tracker;

    while (true) {
        cap >> frame;
        frame_index++;
        std::cout << "************* NEW FRAME ************* " << std::endl;
        tvm::runtime::NDArray input = preprocess_frame(frame, 1, 640, 640);
        set_input("input", input);
        run();
        tvm::runtime::NDArray output = get_output(0);

        std::vector<cv::Rect> all_detections = ProcessYOLOOutput(output, COCO_CLASS, frame, trackedObjects, currentID, 0.45);
        size_t total_frames = all_detections.size();
        std::cout << "total frame is : " << total_frames << std::endl;

        
        // auto frame_index = 1;
        tracker.Run(all_detections);
        const auto tracks = tracker.GetTracks();

        std::cout << "Raw Detection : " << std::endl;
        for(auto &trk : tracks) {
            const auto &bbox = trk.second.GetStateAsBbox();
            if(trk.second.coast_cycles_ < kMaxCoastCycles && (trk.second.hit_streak_ >= kMinHits || frame_index < kMinHits)) {
                std::cout << frame_index << "," << trk.first << "," << bbox.tl().x << "," << bbox.tl().y
                << "," << bbox.width << "," << bbox.height << ",1 ,-1, -1, -1"
                << " Hit Streak = " << trk.second.hit_streak_ << " Coast Cycles = " << trk.second.coast_cycles_ << std::endl; 
            }
        }

        for(const auto &det : all_detections) {
            cv::rectangle(frame, det, cv::Scalar(0, 0, 255), 2);
        }

        for (auto &trk : tracks) {
                    // only draw tracks which meet certain criteria
                    if (trk.second.coast_cycles_ < kMaxCoastCycles &&
                        (trk.second.hit_streak_ >= kMinHits || frame_index < kMinHits)) {
                        const auto &bbox = trk.second.GetStateAsBbox();
                        cv::putText(frame, std::to_string(trk.first), cv::Point(bbox.tl().x, bbox.tl().y - 10),
                                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
                        cv::rectangle(frame, bbox, cv::Scalar(0, 0, 255), 2);
                    }
        }

        cv::imshow(this->model_name, frame);

        if (cv::waitKey(10) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
}

