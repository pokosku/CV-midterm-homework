#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat match_features(std::vector<cv::Keypoint> kp1, std::vector<cv::Keypoint> kp2){
    cv::BFMatcher matcher(cv::NORM_L2);

    std::vector<std::vector<cv::DMatch>> knn_matches;

    matcher.knnMatch(k1, k2, knn_matches, 2);
    cv::Mat output;
    
    //to be tester
    double ratio_thresh = 0.7;
    double movement_thresh = 5.0;
    
    std::vector<cv::KeyPoint> points_on_object;

    for (size_t i = 0; i < knn_matches.size(); i++) {
    if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
        
        
        cv::Point2f p0 = k1[knn_matches[i][0].queryIdx].pt;
        cv::Point2f pN = k2[knn_matches[i][0].trainIdx].pt;

        float dist = cv::norm(p0 - pN);

        if (dist > movement_thresh) {
            points_on_object.push_back(p0);
        }
    }
}
    //cv::drawKeypoints(moving_mask, keypoints, output);
    
    return output;

}

