#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

#include "DataLoader.hpp"
#include "feature_detection.hpp"
#include "feature_matching.hpp"
#include "box_definition.hpp"
#include "IoU_calculation.hpp"
#include "optical_flow.hpp"


cv::Rect SIFT_pipeline(DataLoader dl){

    cv::Mat frame_0 = dl.load_next_img();
    std::vector<cv::KeyPoint> kp_0;
    cv::Mat desc_0;

    detect_features(frame_0, kp_0, desc_0);

    std::vector<cv::Point2f> all_candidate_points;

    while(true){

        cv::Mat frame_t = dl.load_next_img();

        if(frame_t.empty()){
            break;
        }

        std::vector<cv::KeyPoint> kp_t;
        cv::Mat desc_t;
        
        detect_features(frame_t, kp_t, desc_t);

        std::vector<cv::Point2f> points = match_features(desc_0, kp_0, desc_t, kp_t);

        // Appending matched feature points to the candidate features for later clustering
        all_candidate_points.insert(all_candidate_points.end(), points.begin(), points.end());
    }
    
    cv::Rect bbox = bbox_from_clustering(all_candidate_points, frame_0.size());


    dl.reset_head();

    return bbox;
}

cv::Rect optical_flow_pipeline(DataLoader dl){
    int step = dl.get_number_images()/20;

    if(step < 2){
        step = 2;
    }
    
    cv::Mat frame_0 = dl.load_next_img(step);
    cv::Mat motion_mask = get_refined_motion_mask(frame_0, dl.load_next_img());
    cv::Rect bbox = bbox_from_mask(motion_mask);

    dl.reset_head();

    return bbox;
}


int main(int argc, char** argv){

    std::vector<std::string> classes = {"bird", "car", "frog", "sheep", "squirrel"};

    int accuracy_SIFT = 0, accuracy_optical_flow = 0;
    float mean_iou_SIFT = 0, mean_iou_optical_flow = 0;
    
    for(const auto& className : classes){

        // DataLoader 
        DataLoader dl(className);
        
        cv::Rect bbox_SIFT = SIFT_pipeline(dl);

        cv::Rect bbox_optical_flow = optical_flow_pipeline(dl);

        //evaluation
        float iou;

        iou = calculate_IoU(bbox_SIFT, className); 
        
        accuracy_SIFT += iou > 0.5;
        mean_iou_SIFT += iou;

        iou = calculate_IoU(bbox_optical_flow, className); 
        
        accuracy_optical_flow += iou > 0.5;
        mean_iou_optical_flow += iou;
    }

    mean_iou_SIFT /= classes.size();
    mean_iou_optical_flow /= classes.size();

    std::cout <<"FINAL EVALUATION: " << std::endl;
    std::cout << "SIFT ROBUST FEATURE MATCHING: " << std::endl;
    std::cout <<"Mean IoU: " << mean_iou_SIFT<< std::endl;
    std::cout <<"Accuracy: " << accuracy_SIFT<<"/"<<classes.size()<< std::endl;
    std::cout << "OPTICAL FLOW: " << std::endl;
    std::cout <<"Mean IoU: " << mean_iou_optical_flow<< std::endl;
    std::cout <<"Accuracy: " << accuracy_optical_flow<<"/"<<classes.size()<< std::endl;
    
    cv::destroyAllWindows();
    return 0;
}