#ifndef DATALOADER_HPP
#define DATALOADER_HPP
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat LoadImg(string folder, int number);
#endif