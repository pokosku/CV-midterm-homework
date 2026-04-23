#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

#include "DataLoader.hpp"
#include "feature_detection.hpp"
#include "feature_matching.hpp"
#include "box_definition.hpp"

int main(int argc, char** argv){
    DataLoader dl = DataLoader("frog");
    
    
    char pressed_key = 'q';
    cv::Mat frame_0 = dl.load_next_img();
    cv::Mat test = frame_0.clone();
    std::vector<cv::KeyPoint> kp0;
    cv::Mat descriptor0;
    detect_features(frame_0, kp0, descriptor0);
    cv::namedWindow("test");
    
    std::vector<cv::Point2f> all_candidate_points;

    cv::Mat frame_t = dl.load_next_img();
    

    while(!frame_t.empty()){
        std::vector<cv::KeyPoint> kp_t;
        cv::Mat descriptor_t;
        
        detect_features(frame_t, kp_t, descriptor_t);

        std::vector<cv::Point2f> points = match_features(descriptor0, kp0, descriptor_t, kp_t);

        // Appending matched feature points to the candidate features for later clustering
        all_candidate_points.insert(all_candidate_points.end(), points.begin(), points.end());


        // Conversion of matched feature points (Point2f) to KeyPoints
        std::vector<cv::KeyPoint> keypoints;
        for(auto &p : points){
            keypoints.push_back(cv::KeyPoint(p, 1.0));
        }
        
        //cv::Mat output = frame_0.clone();
        //cv::drawKeypoints(frame_0, keypoints, output);
        //cv::imshow("test",output);
        //pressed_key = cv::waitKey(0); 

        frame_t = dl.load_next_img();


    }
    
    cv::Rect bbox = compute_box(all_candidate_points, frame_0);
    cv::rectangle(test, bbox, cv::Scalar(0,0,255));
    
    cv::imshow("test", test);
    cv::waitKey(0);
    return 0;
}