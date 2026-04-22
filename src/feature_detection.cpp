#include "feature_detection.hpp"


cv::Mat detect_features(cv::Mat frame_1, cv::Mat frame_2){

    cv::Mat moving_mask;
    cv::absdiff(frame_1, frame_2, moving_mask);

    cv::threshold(moving_mask, moving_mask,20,255, cv::THRESH_BINARY);
    
    return moving_mask;
}