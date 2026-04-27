#ifndef BOX_DEFINITION_H
#define BOX_DEFINITION_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>

cv::Rect bbox_from_clustering(const std::vector<cv::Point2f>& candidate_point, cv::Size frame_size);
cv::Rect bbox_from_mask(const cv::Mat& mask);
#endif