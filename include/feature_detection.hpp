#ifndef FEATURE_DETECTION_H
#define FEATURE_DETECTION_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

cv::Mat detect_features(cv::Mat frame_1, cv::Mat frame_2);

#endif