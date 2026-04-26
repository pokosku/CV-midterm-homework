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

int main(int argc, char** argv){

    std::vector<std::string> classes = {"bird", "car", "frog", "sheep", "squirrel"};

    int accuracy = 0;
    float mean_IoU = 0;
    
    for(auto &c : classes){

        DataLoader dl = DataLoader(c);

        // initial feature detection of frame 0
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

        //stampa debug
        std::cout << "Numero punti candidati: " << all_candidate_points.size() << std::endl;
        
        cv::Rect bbox = define_bounding_box(all_candidate_points, frame_0.size());

        for(auto &p : all_candidate_points){
            cv::circle(frame_0, p, 1, cv::Scalar(0,0,255), 1);
        }

        //evaluation
        float IoU = calculate_IoU(bbox, c);
        std::cout <<"IoU on " << c << ": "<< IoU << std::endl;
        accuracy += IoU > 0.5;
        mean_IoU += IoU;

        cv::rectangle(frame_0, bbox, cv::Scalar(0,0,255));
        cv::namedWindow("rect", cv::WINDOW_NORMAL);
        cv::imshow("rect", frame_0);
        cv::waitKey(0);
    }

    mean_IoU /= classes.size();

    std::cout <<"FINAL EVALUATION: " << std::endl;
    std::cout <<"Mean IoU: " << mean_IoU<< std::endl;
    std::cout <<"Accuracy: " << accuracy<<"/"<<classes.size()<< std::endl;
    
    cv::destroyAllWindows();
    return 0;
}