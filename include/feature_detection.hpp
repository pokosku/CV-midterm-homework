#ifndef FEATURE_DETECTION_H
#define FEATURE_DETECTION_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

void detect_features(const cv::Mat& frame, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors);


#endif