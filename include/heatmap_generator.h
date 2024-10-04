#ifndef _HEATMAP_GENERATOR_ 
#define _HEATMAP_GENERATOR_

#include <opencv2/opencv.hpp>

class HeatMapGenerator {
    public:
        HeatMapGenerator();
        ~HeatMapGenerator();

        cv::Mat generateHeatmap(cv::Mat& frame, int x1, int y1, int x2, int y2);

};


#endif
