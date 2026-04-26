// AUTHORS: Leonardo Joao Fabbro

#include "feature_detection.hpp"

void detect_features(const cv::Mat& frame, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors){
    
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    // We obtained the relative best parameter after performing some manual tuning
    cv::Ptr<cv::SIFT> sift = cv::SIFT::create(0,3, 0.04, 100.0, 1.45);
    // We find the keypoints and compute the descriptor at the same time
    sift -> detectAndCompute(gray, cv::noArray(), keypoints, descriptors);

}
