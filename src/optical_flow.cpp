#include "optical_flow.hpp"


cv::Mat get_refined_motion_mask(const cv::Mat& frame1, const cv::Mat& frame2) {
 cv::Mat gray1, gray2;
    
    cv::cvtColor(frame1, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(frame2, gray2, cv::COLOR_BGR2GRAY);
    
    
    cv::GaussianBlur(gray1, gray1, cv::Size(3, 3), 0);
    cv::GaussianBlur(gray2, gray2, cv::Size(3, 3), 0);

   
    cv::Mat flow;
    
    cv::calcOpticalFlowFarneback(gray1, gray2, flow, 0.3, 3, 12, 3, 3, 1.1, 0);

    // Magnitude division X[0], Y[1] 
    cv::Mat flow_parts[2];
    cv::split(flow, flow_parts); 
    
    cv::Mat magnitude, angle;
    cv::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);

    // Checking for no-movement
    double min_val, max_val;
    cv::minMaxLoc(magnitude, &min_val, &max_val);
    if (max_val < 1.0) { 
        return cv::Mat::zeros(gray1.size(), CV_8U);
    }

    // Thresholding
    cv::Mat mag_norm, mask;
    
    cv::normalize(magnitude, mag_norm, 0, 255, cv::NORM_MINMAX);
    mag_norm.convertTo(mag_norm, CV_8U);
    
    
    cv::threshold(mag_norm, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

   // Opening and closing using morphological operators to clean 
    cv::Mat kOpen = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::Mat kClose = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 21));
    
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kOpen);   
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kClose); 

    return mask;
}
