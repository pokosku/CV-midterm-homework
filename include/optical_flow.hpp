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

cv::Mat get_refined_motion_mask(const cv::Mat& frame1, const cv::Mat& frame2);


#endif