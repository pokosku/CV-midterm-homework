#include "box_definition.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>


cv::Rect define_bounding_box(const std::vector<cv::Point2f>& points, cv::Size frame_size) {
    if (points.empty()) return cv::Rect(0, 0, 0, 0);

    // 1. Clustering spaziale
    std::vector<int> labels;

    // Soglia di distanza: se due punti sono entro 40 pixel, sono "parenti"
    float dist_threshold = 40.0f; 
    
    int n_clusters = cv::partition(points, labels, [dist_threshold](const cv::Point2f& a, const cv::Point2f& b) {
        return cv::norm(a - b) < dist_threshold;
    });

    // 2. Trova il cluster più numeroso
    std::vector<int> cluster_counts(n_clusters, 0);
    for (int label : labels) {
        cluster_counts[label]++;
    }

    int best_cluster_idx = std::distance(cluster_counts.begin(),  std::max_element(cluster_counts.begin(), cluster_counts.end()));

    // 3. Estrai i punti appartenenti al miglior cluster
    std::vector<cv::Point2f> best_cluster_points;
    for (size_t i = 0; i < points.size(); ++i) {
        if (labels[i] == best_cluster_idx) {
            best_cluster_points.push_back(points[i]);
        }
    }

    // 4. Calcola la Bounding Box
    if (best_cluster_points.size() < 4) return cv::Rect(0, 0, 0, 0); // Evita cluster troppo piccoli
    return cv::boundingRect(best_cluster_points);
}

cv::Rect get_smart_bbox(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    
    // Trova i contorni degli "ammassi" bianchi nella maschera
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        return cv::Rect(0, 0, 0, 0);
    }

    double max_area = 0;
    cv::Rect best_box(0, 0, 0, 0);

    // Troviamo SOLO il contorno con l'area maggiore (il soggetto principale)
    for (const auto& contour : contours) {
        double current_area = cv::contourArea(contour);
        
        if (current_area > max_area) {
            max_area = current_area;
            best_box = cv::boundingRect(contour);
        }
    }

    // Ignoriamo tutto se perfino il contorno più grande è solo polvere/rumore
    if (max_area < 150) {
        return cv::Rect(0, 0, 0, 0);
    }

    // Restituiamo strettamente la box del soggetto principale, 
    // ignorando eventuali altri contorni lontani.
    return best_box;

}
