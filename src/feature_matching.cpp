#include "feature_matching.hpp"
#include <opencv2/calib3d.hpp> // Necessario per findHomography

std::vector<cv::Point2f> match_features(const cv::Mat& desc1, const std::vector<cv::KeyPoint> kp1, const cv::Mat& desc2, const std::vector<cv::KeyPoint> kp2){
    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher.knnMatch(desc1, desc2, knn_matches, 2);

    double ratio_thresh = 0.7; 
    
    std::vector<cv::Point2f> src_pts;
    std::vector<cv::Point2f> dst_pts;

    // 1. Filtro di Lowe per isolare i match robusti
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
            src_pts.push_back(kp1[knn_matches[i][0].queryIdx].pt);
            dst_pts.push_back(kp2[knn_matches[i][0].trainIdx].pt);
        }
    }   

    std::vector<cv::Point2f> points_on_object;

    // 2. Calcolo dell'Omografia con RANSAC se ci sono abbastanza punti
    if(src_pts.size() >= 4) {
        std::vector<uchar> inliers_mask;
        // La soglia di 3.0 pixel qui assorbe l'errore del SIFT e i micro-movimenti (inlier)
        cv::findHomography(src_pts, dst_pts, cv::RANSAC, 3.0, inliers_mask);

        for(size_t i = 0; i < inliers_mask.size(); i++) {
            // Se la maschera è 0, il punto NON fa parte del background
            if(inliers_mask[i] == 0) {
                points_on_object.push_back(src_pts[i]);
            }
        }
    }

    return points_on_object;
}