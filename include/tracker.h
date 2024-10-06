#ifndef __TRACKER__
#define __TRACKER__

#include <opencv2/opencv.hpp>


class Tracker {
    public:
        cv::KalmanFilter baseKalman();
        double mahalanobis_distance(const cv::Point predicted_center, const cv::Point detected_center, const cv::Mat& invCovariance);


};

#endif // __TRACKER__