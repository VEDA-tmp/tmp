#include "heatmap_generator.h"

HeatMapGenerator::HeatMapGenerator() {

}

HeatMapGenerator::~HeatMapGenerator() {
    
}

// cv::Mat HeatMapGenerator::generateHeatmap(cv::Mat& frame, int x1, int y1, int x2, int y2)
// {
//     cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255), 1);
// }

// IoU(Intersection over Union) 계산 함수
float HeatMapGenerator::IOU(const cv::Rect& box1, const cv::Rect& box2) {
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
std::vector<int> HeatMapGenerator::NonMaximumSuppression(const std::vector<cv::Rect>& boxes, const std::vector<float>& confidences, float iou_threshold) {
    std::vector<int> indices;
    std::vector<int> sorted_indices(boxes.size());
    
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
            if (IOU(boxes[idx], boxes[next_idx]) > iou_threshold) {
                suppressed[next_idx] = true;
            }
        }
    }
    return indices;
}