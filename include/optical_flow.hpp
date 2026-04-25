#ifndef OPTICAL_FLOW_H
#define OPTICAL_FLOW_H
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

std::vector<float> track_features(const std::vector<cv::Mat>& frames, const std::vector<cv::Point2f>& p0);
std::vector<cv::Point2f> filter_moving_points(const std::vector<cv::Point2f>& p0, 
                                              const std::vector<float>& displacements, 
                                              cv::Size img_size);
#endif