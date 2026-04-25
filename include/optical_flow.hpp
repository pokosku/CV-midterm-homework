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

std::vector<cv::Point2f> optical_flow_tracking(const cv::Mat& frame1,const cv::Mat& frame2,
     const std::vector<cv::KeyPoint>& kp1,double movement_thresh);
std::vector<cv::Point2f> optical_flow_consecutive(const std::vector<cv::Mat>& frames,
                                const std::vector<cv::KeyPoint>& kp0,double movement_thresh);

#endif