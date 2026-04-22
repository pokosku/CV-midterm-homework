#ifndef DATALOADER_HPP
#define DATALOADER_HPP
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
class DataLoader{

    private:
    const std::string PATHIMAGES="../assets/data/";
    const std::string BIRD="bird";
    const std::string CAR="car";
    const std::string FROG="frog";
    const std::string SHEEP="sheep";
    const std::string SQUIRREL="squirrel";
    std::string folder;
    std::string path;
    std::size_t number_images;
    std::vector<String> folder_images;
    string number_string;
    Mat img;
    string zeros;
    int index;
    string extension;


    public:
    DataLoader(std::string directory);
    Mat LoadFirst();
    Mat LoadNext();

};

#endif