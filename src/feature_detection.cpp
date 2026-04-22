#include "feature_detection.hpp"


cv::Mat detect_features(cv::Mat frame_1, cv::Mat frame_2){

    // preprocessing: we compute image absolute difference 
    cv::Mat moving_mask = cv::Mat::zeros(frame_1.rows, frame_1.cols, CV_8UC1);
    cv::absdiff(frame_1, frame_2, moving_mask);
    //cv::cvtColor(moving_mask, moving_mask, cv::COLOR_BGR2GRAY);
    cv::threshold(moving_mask, moving_mask,20,255, cv::THRESH_BINARY);
    
    cv::Ptr<cv::SIFT> sift_detector = cv::SIFT::create();
    std::vector<cv::KeyPoint> keypoints;
    sift_detector->detect(moving_mask, keypoints);

    cv::Mat output;

    cv::drawKeypoints(moving_mask, keypoints, output);
    
    return output;


}