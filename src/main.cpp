#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "DataLoader.hpp"
#include <iostream>

int main(int argc, char** argv){
    const string NAMEWINDOW="win";
    Mat img=LoadImg("bird",1);
    namedWindow(NAMEWINDOW,WINDOW_NORMAL);
    imshow(NAMEWINDOW,img);
    waitKey(0);
    return 0;
}