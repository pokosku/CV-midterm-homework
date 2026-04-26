// AUTHORS: Leonardo Joao Fabbro

#include "feature_matching.hpp"
#include <opencv2/calib3d.hpp>

std::vector<cv::Point2f> match_features(const cv::Mat& desc1, const std::vector<cv::KeyPoint> kp1, const cv::Mat& desc2, const std::vector<cv::KeyPoint> kp2){
    // BruteForce Matcher for 
    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher.knnMatch(desc1, desc2, knn_matches, 2);

    // We use Lowe's ratio (0.7~0.8) to decide a succesful match between features
    double ratio_thresh = 0.7; 
    
    std::vector<cv::Point2f> src_pts;
    std::vector<cv::Point2f> dst_pts;

    // We compute the matched points from source to destination frame
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
            src_pts.push_back(kp1[knn_matches[i][0].queryIdx].pt);
            dst_pts.push_back(kp2[knn_matches[i][0].trainIdx].pt);
        }
    }   

    std::vector<cv::Point2f> points_on_object;

    // We exploit homography (small camera movement compensation)
    if(src_pts.size() >= 4) {
        std::vector<uchar> inliers_mask;
        // Small RANSAC threshold (3.0) to compensate SIFT mismatch
        cv::findHomography(src_pts, dst_pts, cv::RANSAC, 3.0, inliers_mask);
        // The inliers mask is a vector with the i-th component equal to 1 if the point follows the homography, 0 otherwise
        for(size_t i = 0; i < inliers_mask.size(); i++) {
            if(inliers_mask[i] == 0) {
                // We add points not respecting homography, which are likely to belong to the moving object.
                points_on_object.push_back(src_pts[i]);
            }
        }
    }

    return points_on_object;
}