#include "feature_matching.hpp"

std::vector<cv::Point2f> match_features(const cv::Mat& desc1,const std::vector<cv::KeyPoint> kp1, const cv::Mat& desc2, const std::vector<cv::KeyPoint> kp2){
    cv::BFMatcher matcher(cv::NORM_L2);

    std::vector<std::vector<cv::DMatch>> knn_matches;

    // descriptors are stored in Mat
    matcher.knnMatch(desc1, desc2, knn_matches, 2);
    cv::Mat output;
    
    //to be tested
    double ratio_thresh = 0.7;
    double movement_thresh = 5.0;
    
    std::vector<cv::Point2f> points_on_object;

    for (size_t i = 0; i < knn_matches.size(); i++) {
    if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
        
        
        cv::Point2f p0 = kp1[knn_matches[i][0].queryIdx].pt;
        cv::Point2f pN = kp2[knn_matches[i][0].trainIdx].pt;

        float dist = cv::norm(p0 - pN);

        if (dist > movement_thresh) {
            points_on_object.push_back(p0);
        }
    }
}
    //cv::drawKeypoints(moving_mask, keypoints, output);
    
    return points_on_object;

}

