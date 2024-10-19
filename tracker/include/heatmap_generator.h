#ifndef _HEATMAP_GENERATOR_ 
#define _HEATMAP_GENERATOR_

#include <opencv2/opencv.hpp>
#include <vector>

class HeatMapGenerator {
    public:
        HeatMapGenerator();
        ~HeatMapGenerator();

        // cv::Mat generateHeatmap(cv::Mat& frame, int x1, int y1, int x2, int y2);
        float IOU(const cv::Rect& box1, const cv::Rect& box2);
        std::vector<int> NonMaximumSuppression(const std::vector<cv::Rect>& boxes, const std::vector<float>& confidences, float iou_threshold);
};


#endif
