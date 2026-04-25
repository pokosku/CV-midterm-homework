#include "optical_flow.hpp"

// Resta invariata, utile per confronti veloci tra due frame
std::vector<cv::Point2f> optical_flow_tracking(const cv::Mat& frame1, const cv::Mat& frame2,
     const std::vector<cv::KeyPoint>& kp1, double movement_thresh){
    
    cv::Mat gray_1, gray_2;
    cv::cvtColor(frame1, gray_1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(frame2, gray_2, cv::COLOR_BGR2GRAY);

    std::vector<cv::Point2f> points1;
    cv::KeyPoint::convert(kp1, points1);

    std::vector<cv::Point2f> points2;
    std::vector<uchar> status;
    std::vector<float> errors;

    cv::calcOpticalFlowPyrLK(gray_1, gray_2, points1, points2, status, errors, cv::Size(21,21), 3);

    std::vector<cv::Point2f> moving_points;
    for (int i = 0; i < points1.size(); i++) {
        if (status[i] == 1) { 
            float dist = cv::norm(points1[i] - points2[i]);
            if (dist > movement_thresh) {
                moving_points.push_back(points1[i]); 
            }
        }
    }
    return moving_points;
}

std::vector<cv::Point2f> optical_flow_consecutive(
                                const std::vector<cv::Mat>& frames,
                                const std::vector<cv::KeyPoint>& kp0,
                                double movement_thresh) {

    std::vector<cv::Point2f> current_points;
    cv::KeyPoint::convert(kp0, current_points);
    std::vector<cv::Point2f> original_points = current_points;
    
    // Sostituiamo il boolean con un contatore di "eventi dinamici"
    std::vector<int> dynamic_count(current_points.size(), 0);
    // Teniamo traccia di quante volte ogni punto è stato effettivamente tracciato
    std::vector<int> appearance_count(current_points.size(), 0);
    std::vector<bool> valid(current_points.size(), true);

    for (int i = 0; i < (int)frames.size() - 1; i++) {
        cv::Mat gray1, gray2;
        cv::cvtColor(frames[i], gray1, cv::COLOR_BGR2GRAY);
        cv::cvtColor(frames[i+1], gray2, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> pts_to_track, pts_next;
        std::vector<int> valid_indices;
        
        for (int j = 0; j < (int)current_points.size(); j++) {
            if (valid[j]) {
                pts_to_track.push_back(current_points[j]);
                valid_indices.push_back(j);
            }
        }

        if (pts_to_track.size() < 4) break; 

        std::vector<uchar> status;
        std::vector<float> errors;
        cv::calcOpticalFlowPyrLK(gray1, gray2, pts_to_track, pts_next, status, errors, cv::Size(21,21), 3);

        std::vector<uchar> mask;
        // Calcoliamo il movimento globale (sfondo)
        cv::findHomography(pts_to_track, pts_next, cv::RANSAC, 3.0, mask);

        for (int j = 0; j < (int)valid_indices.size(); j++) {
            int idx = valid_indices[j];
            if (status[j] == 1) {
                current_points[idx] = pts_next[j];
                appearance_count[idx]++; // Il punto è ancora visibile

                // SE è un outlier del RANSAC, allora si sta muovendo diversamente dallo sfondo
                if (mask.empty() || mask[j] == 0) {
                    // movement_thresh qui aiuta a ignorare micro-vibrazioni
                    if (cv::norm(pts_to_track[j] - pts_next[j]) > 0.5) { 
                        dynamic_count[idx]++;
                    }
                }
            } else {
                valid[idx] = false;
            }
        }
    }

    std::vector<cv::Point2f> moving_points;
    for (int i = 0; i < (int)original_points.size(); i++) {
        if (appearance_count[i] > 0) {
            // Un punto è "buono" se è risultato dinamico per almeno il 30% della sua vita
            float dynamic_ratio = (float)dynamic_count[i] / (float)appearance_count[i];
            
            if (dynamic_ratio > 0.3f) { 
                moving_points.push_back(original_points[i]);
            }
        }
    }

    return moving_points;
}