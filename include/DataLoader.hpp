#ifndef DATALOADER_HPP
#define DATALOADER_HPP
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

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
    std::vector<cv::String> folder_images;
    std::string number_string;
    cv::Mat img;
    std::string zeros;
    int index;
    std::string extension;


    public:
    DataLoader(std::string directory);
    cv::Mat load_test_img();
    cv::Mat load_next_img();
    cv::Mat load_next_img(int step);

};

#endif