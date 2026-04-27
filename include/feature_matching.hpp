#ifndef FEATURE_MATCHING_H
#define FEATURE_MATCHING_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

std::vector<cv::Point2f> match_features(const cv::Mat& desc1,const std::vector<cv::KeyPoint> kp1, const cv::Mat& desc2, const std::vector<cv::KeyPoint> kp2);

#endif