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
    cv::Mat frame_0 = dl.load_next_img();
    //cv::Mat test = img1.clone();
    std::vector<cv::KeyPoint> kp0;
    cv::Mat descriptor0;
    detect_features(frame_0, kp0, descriptor0);
    cv::namedWindow("test");
    
    while(pressed_key != 'x'){
        std::vector<cv::KeyPoint> kp_t;
        cv::Mat descriptor_t;
        cv::Mat frame_t = dl.load_next_img();

        
        detect_features(frame_t, kp_t, descriptor_t);

        std::vector<cv::Point2f> points = match_features(descriptor0, kp0, descriptor_t, kp_t);
        std::vector<cv::KeyPoint> keypoints;

        for(auto &p : points){
            keypoints.push_back(cv::KeyPoint(p, 1.0));
        }
        
        cv::Mat output = frame_0.clone();
        cv::drawKeypoints(frame_0, keypoints, output);
        cv::imshow("test",output);
        pressed_key = cv::waitKey(0); 

    }
    
    return 0;
}