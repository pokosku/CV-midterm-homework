#ifndef IOU_CALCULATION_H
#define IOU_CALCULATION_H
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

float calculateIoU(cv::Rect rectA,std::string folder);

#endif