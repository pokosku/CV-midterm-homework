// AUTHORS: Mirco Zavarise, Daniele Riolmi Rossetto, Leonardo Joao Fabbro

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

// Pipeline for the SIFT procedure
cv::Rect SIFT_pipeline(DataLoader dl){

    cv::Mat frame_0 = dl.load_next_img();
    std::vector<cv::KeyPoint> kp_0;
    cv::Mat desc_0;

    // Detecting features and descriptors from the first frame
    detect_features(frame_0, kp_0, desc_0);

    std::vector<cv::Point2f> all_candidate_points;

    // Cicle for all the frames
    while(true){

        // Loading next frame
        cv::Mat frame_t = dl.load_next_img();

        // Checking for emptiness of the frame
        if(frame_t.empty()){
            break;
        }
        
        std::vector<cv::KeyPoint> kp_t;
        cv::Mat desc_t;
        
        // Detecting features and descriptors from the current frame
        detect_features(frame_t, kp_t, desc_t);

        std::vector<cv::Point2f> points = match_features(desc_0, kp_0, desc_t, kp_t);

        // Appending matched feature points to the candidate features for later clustering
        all_candidate_points.insert(all_candidate_points.end(), points.begin(), points.end());
    }

    // Computing bounding box for first frame
    cv::Rect bbox = bbox_from_clustering(all_candidate_points, frame_0.size());

    // DataLoader index reset
    dl.reset_head();

    return bbox;
}

// Pipeline for the optical flow procedure
cv::Rect optical_flow_pipeline(DataLoader dl){

    // Choosing dynamically the step between selected frame
    int step = dl.get_number_images()/20;

    // Rounding up step if size too small
    if(step < 2){
        step = 2;
    }
    
    // Loading second frame according to step
    cv::Mat frame_0 = dl.load_next_img(step);
    // Computing mask from optical flow
    cv::Mat motion_mask = get_refined_motion_mask(frame_0, dl.load_next_img());
    // Computing bounding box for first frame
    cv::Rect bbox = bbox_from_mask(motion_mask);

    // DataLoader index reset
    dl.reset_head();

    return bbox;
}


int main(int argc, char** argv){


    // String vector for directories 
    std::vector<std::string> classes = {"bird", "car", "frog", "sheep", "squirrel"};

    // Initialization of accuracy and intersection over union for both pipelines
    int accuracy_SIFT = 0, accuracy_optical_flow = 0;
    float mean_iou_SIFT = 0, mean_iou_optical_flow = 0;
    
    // Cicle for all the classes
    for(const auto& className : classes){

        // DataLoader initialization
        DataLoader dl(className);
        
        // Calling SIFT pipeline
        cv::Rect bbox_SIFT = SIFT_pipeline(dl);

        // Calling optical flow pipeline
        cv::Rect bbox_optical_flow = optical_flow_pipeline(dl);

        // Evaluation of accuracy and intersection over union for both pipelines
        float iou;

        iou = calculate_IoU(bbox_SIFT, className); 
        
        accuracy_SIFT += iou > 0.5;
        mean_iou_SIFT += iou;

        generate_output(bbox_SIFT,dl.load_test_img(),"SIFT",className);

        iou = calculate_IoU(bbox_optical_flow, className); 
        
        accuracy_optical_flow += iou > 0.5;
        mean_iou_optical_flow += iou;

        generate_output(bbox_optical_flow,dl.load_test_img(),"optical_flow",className);
    }

    // Calculating final mean intersection over union among classes for both methods 
    mean_iou_SIFT /= classes.size();
    mean_iou_optical_flow /= classes.size();

    // Printing final values
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