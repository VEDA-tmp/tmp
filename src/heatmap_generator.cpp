#include "heatmap_generator.h"

HeatMapGenerator::HeatMapGenerator() {

}

HeatMapGenerator::~HeatMapGenerator() {
    
}

cv::Mat HeatMapGenerator::generateHeatmap(cv::Mat& frame, int x1, int y1, int x2, int y2)
{
    cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255), 1);
}

