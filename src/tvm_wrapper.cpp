#include "tvm_wrapper.h"


cv::VideoCapture iphone_init(double frame_w, double frame_h, double fps) {
    LOG(INFO) << "Camera SetUp";

    cv::VideoCapture cap(0, cv::CAP_AVFOUNDATION); 
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera." << std::endl;
        EXIT_FAILURE;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, frame_w);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, frame_h);
    cap.set(cv::CAP_PROP_FPS, fps);

    return cap;
}

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

void ProcessYOLOOutput(tvm::runtime::NDArray output, int output_w, int output_h, const std::vector<std::string>& class_names, cv::Mat& frame, float conf_threshold = 0.55) {
    const int64_t* shape = output.Shape().data();
    int num_detections = shape[1];

    float* output_data = static_cast<float*>(output->data);
    int num_classes = class_names.size();

    int original_width = frame.cols;
    int original_height = frame.rows;

    const float width_ratio = static_cast<float>(original_width) / float(output_w);
    const float height_ratio = static_cast<float>(original_width) / float(output_h);

    const int data_stride = 5 + num_classes;

        for (int i = 0; i < num_detections; ++i) {
        int base_index = i * data_stride;
        
        // 위치 정보와 신뢰도를 가져옵니다. 화면의 비율과 모델의 input 비율이 다를 수 있기 때문에 ratio를 설정했습니다.
        float cx = output_data[base_index + 0] * width_ratio;
        float cy = output_data[base_index + 1] * height_ratio;
        float w = output_data[base_index + 2] * width_ratio;
        float h = output_data[base_index + 3] * height_ratio;
        float confidence = output_data[base_index + 4];

        if (confidence > conf_threshold) {
            // 클래스 점수(confidence) 값 가져오기
            float* class_scores = &output_data[base_index + 5];
            int class_id = std::distance(class_scores, std::max_element(class_scores, class_scores + num_classes));

            // 바운딩 박스 좌표 계산
            int x1 = static_cast<int>(cx - (w / 2));
            int y1 = static_cast<int>(cy - (h / 2));
            int x2 = static_cast<int>(cx + (w / 2));
            int y2 = static_cast<int>(cy + (h / 2));

            // 바운딩 박스 그리기
            cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);

            // 클래스 이름과 confidence 출력합니다.
            // 추후에 IOU와 NMS를 설정해 비슷한 바운더리에 대해서 후처리 진행할 예정입니다.
            std::string label = class_names[class_id] + " " + cv::format("%.2f", confidence);
            cv::putText(frame, label, cv::Point(x1, y1 - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        }
    }

    cv::imshow("YOLOv5 Detection", frame);
}

// AI 모델을 불러오고 처리하는 메인 로직입니다.
void TVMWrapper::detect() {
    LOG(INFO) << this->model_name + "Model Setup...";
    tvm::runtime::PackedFunc set_input = mod.GetFunction("set_input");
    tvm::runtime::PackedFunc run = mod.GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod.GetFunction("get_output");
    void (*output_processing)(tvm::runtime::NDArray, int output_w, int output_h, const std::vector<std::string>&, cv::Mat&, float threshold);
    
    if(this->model_name == "yolov5") {
        output_processing = ProcessYOLOOutput;
    }
    
    cv::VideoCapture cap = iphone_init(640, 640 ,15);
    cv::Mat frame;

    while(true) {
        cap >> frame;
        tvm::runtime::NDArray input = preprocess_frame(frame, 1, 640, 640);
        
        set_input("input", input);
        run();

        tvm::runtime::NDArray output = get_output(0);
        output_processing(output, 640, 640, COCO_CLASS, frame, 0.45);

        cv::imshow(this->model_name, frame);
        
        if(cv::waitKey(10) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
}