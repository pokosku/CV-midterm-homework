#ifndef FEATURE_DETECTION_H
#define FEATURE_DETECTION_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

cv::Rect compute_box(std::vector<cv::Point2f>& candidate_point, cv::Mat& frame1);
#endif