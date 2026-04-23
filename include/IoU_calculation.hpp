#ifndef IOU_CALCULATION_H
#define IOU_CALCULATION_H
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

float calculate_IoU(cv::Rect rect_a,std::string folder);

#endif