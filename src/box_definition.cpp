#include "box_definition.hpp"

cv::Rect compute_box(std::vector<cv::Point2f>& candidate_points, cv::Mat& frame1){

    cv:: Mat heatmap = cv::Mat::zeros(frame1.rows, frame1.cols , CV_32F);

    std::cout << "Frame size: " << frame1.cols << "x" << frame1.rows << std::endl;
    std::cout << "Punti dentro i bordi: " << std::endl;
    int count_valid = 0;
    for (const auto& pt : candidate_points) {
        if(pt.x >= 0 && pt.x < frame1.cols && pt.y >= 0 && pt.y < frame1.rows){
            count_valid++;
        }
    }
    std::cout << "Punti validi: " << count_valid << " su " << candidate_points.size() << std::endl;

    std::cout << "Primi 20 punti: " << std::endl;
    for (int i = 0; i < std::min(20, (int)candidate_points.size()); i++) {
        std::cout << candidate_points[i].x << " " << candidate_points[i].y << std::endl;
    }

    for (const auto &pt : candidate_points){
    if(pt.x >= 0 && pt.x < frame1.cols && pt.y >= 0 && pt.y < frame1.rows){
        // crea un cerchio temporaneo e aggiungilo alla heatmap
        cv::Mat temp = cv::Mat::zeros(frame1.rows, frame1.cols, CV_32F);
        cv::circle(temp, pt, 40, cv::Scalar(1.0), -1);
        cv::add(heatmap, temp, heatmap);
    }
}

    // dopo il ciclo dei cerchi
    double minVal, maxVal;
    cv::minMaxLoc(heatmap, &minVal, &maxVal);
    std::cout << "Heatmap max value: " << maxVal << std::endl;

    cv::Mat heatmap_vis;
    cv::normalize(heatmap, heatmap_vis, 0, 255, cv::NORM_MINMAX);
    heatmap_vis.convertTo(heatmap_vis, CV_8U);
    cv::imwrite("heatmap_debug.jpg", heatmap_vis);

    cv::Mat binary;
    cv::threshold(heatmap, binary, 150.0, 255, cv::THRESH_BINARY); 
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