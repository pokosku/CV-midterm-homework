#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

#include "DataLoader.hpp"
#include "feature_detection.hpp"
#include "feature_matching.hpp"
#include "optical_flow.hpp"
#include "box_definition.hpp"
#include "IoU_calculation.hpp"

int main(int argc, char** argv){

    std::string folder = "frog";
    DataLoader dl = DataLoader(folder);

    cv::Mat frame_0 = dl.load_next_img();
    cv::Mat test = frame_0.clone();
    std::vector<cv::KeyPoint> kp0;
    cv::Mat descriptor0;
    detect_features(frame_0, kp0, descriptor0);
    cv::namedWindow("test");

    // carica tutti i frame in un vettore
    std::vector<cv::Mat> frames;
    frames.push_back(frame_0);
    cv::Mat frame_t = dl.load_next_img();
    while (!frame_t.empty()) {
        frames.push_back(frame_t);
        frame_t = dl.load_next_img();
    }

    std::cout << "Numero frame caricati: " << frames.size() << std::endl;

    // tracka con optical flow consecutivo
    std::vector<cv::Point2f> all_candidate_points = optical_flow_consecutive(
        frames,
        kp0,
        5.0  // soglia sullo spostamento accumulato
    );

    std::cout << "Numero punti candidati: " << all_candidate_points.size() << std::endl;

    cv::Rect bbox = compute_box(all_candidate_points, frame_0);
    std::cout << "Bbox: " << bbox.x << " " << bbox.y << " " << bbox.width << " " << bbox.height << std::endl;

    std::cout<<"IoU found:"<<calculate_IoU(cv::Rect( bbox.x, bbox.y,bbox.x+bbox.width,bbox.y+bbox.height),folder)<<std::endl;

    cv::rectangle(test, bbox, cv::Scalar(0,0,255));
    cv::imshow("test", test);
    cv::waitKey(0);
    return 0;
}