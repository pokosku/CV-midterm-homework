#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

#include "DataLoader.hpp"
#include "feature_detection.hpp"
#include "feature_matching.hpp"

int main(int argc, char** argv){
    DataLoader dl = DataLoader("frog");
    
    
    char pressed_key = 'q';
    cv::Mat img1 = dl.load_next_img(10);
    cv::Mat test = img1.clone();

    cv::namedWindow("test");
    
    while(pressed_key != 'x'){
        std::vector<cv::KeyPoint> kp1,kp2;
        cv::Mat descriptor1, descriptor2;
        cv::Mat img2 = dl.load_next_img(10);

        detect_features(img1, kp1, descriptor1);
        detect_features(img2, kp2, descriptor2);

        std::vector<cv::Point2f> points = match_features(descriptor1, kp1, descriptor2, kp2);
        std::vector<cv::KeyPoint> keypoints;

        for(auto &p : points){
            keypoints.push_back(cv::KeyPoint(p, 1.0));
        }
        
        cv::Mat output = test.clone();
        cv::drawKeypoints(test, keypoints, output);
        cv::imshow("test",output);
        pressed_key = cv::waitKey(0);
        
        img1 = img2.clone();
        

    }
    
    return 0;
}