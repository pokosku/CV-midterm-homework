#include "feature_detection.hpp"

void detect_features_sift(const cv::Mat& frame, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors){

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create(1000, 3, 0.04, 10, 1.6);
    sift -> detectAndCompute(gray, cv::noArray(), keypoints, descriptors);

}
