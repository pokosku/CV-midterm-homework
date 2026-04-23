#include "box_definition.hpp"

cv::Rect compute_box(std::vector<cv::Point2f>& candidate_points, cv::Mat& frame1){

    cv:: Mat heatmap = cv::Mat::zeros(frame1.rows, frame1.cols , CV_32F);

    for (const auto &pt : candidate_points){
        if(pt.x >=0 && pt.x<frame1.cols && pt.y >0 && pt.y < frame1.rows){
            cv::circle(heatmap, pt, 15, cv::Scalar(1.0), -1);
        }
    }

    cv::Mat binary;
    cv::threshold(heatmap, binary, 3.0, 255, cv::THRESH_BINARY);
    binary.convertTo(binary, CV_8U);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        // no box
        return cv::Rect();
    }

    int largest = 0;
    for (int i = 1; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) > cv::contourArea(contours[largest])) {
            largest = i;
        }
    }

    return cv::boundingRect(contours[largest]);

}