#include "box_definition.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>

// Funzione helper per calcolare la mediana di un vettore di float
float get_median(std::vector<float>& values) {
    if (values.empty()) return 0.0f;
    size_t n = values.size() / 2;
    std::nth_element(values.begin(), values.begin() + n, values.end());
    return values[n];
}

cv::Rect compute_box(std::vector<cv::Point2f>& candidate_points, cv::Mat& frame1) {
    if (candidate_points.empty()) {
        return cv::Rect(0, 0, 0, 0);
    }

    // 1. Estrazione coordinate
    std::vector<float> x_coords, y_coords;
    for (const auto& p : candidate_points) {
        x_coords.push_back(p.x);
        y_coords.push_back(p.y);
    }

    // 2. Calcolo Mediana (Centro Robusto)
    std::vector<float> x_copy = x_coords;
    std::vector<float> y_copy = y_coords;
    float medianX = get_median(x_copy);
    float medianY = get_median(y_copy);

    // 3. Calcolo del MAD (Median Absolute Deviation)
    // È l'equivalente robusto della deviazione standard
    std::vector<float> diffX, diffY;
    for (float x : x_coords) diffX.push_back(std::abs(x - medianX));
    for (float y : y_coords) diffY.push_back(std::abs(y - medianY));
    
    float madX = get_median(diffX);
    float madY = get_median(diffY);

    // 4. Filtraggio Outlier basato su MAD
    // Una soglia tipica è tra 2.0 e 3.0. Più è bassa, più la box è "stretta" sul nucleo dei punti.
    std::vector<cv::Point2f> filtered_points;
    float multiplier = 2.5f; 

    for (const auto& p : candidate_points) {
        if (std::abs(p.x - medianX) < multiplier * madX &&
            std::abs(p.y - medianY) < multiplier * madY) {
            filtered_points.push_back(p);
        }
    }

    // Fallback se il filtro è troppo severo
    if (filtered_points.size() < 3) {
        filtered_points = candidate_points;
    }

    // 5. Calcolo Bounding Box
    cv::Rect box = cv::boundingRect(filtered_points);

    // 6. Padding Dinamico
    // Invece di un 10% fisso, usiamo un piccolo margine per compensare il fatto 
    // che le feature sono spesso interne ai bordi dell'oggetto.
    int padding_w = static_cast<int>(box.width * 0.15);
    int padding_h = static_cast<int>(box.height * 0.15);
    
    box.x = std::max(0, box.x - padding_w);
    box.y = std::max(0, box.y - padding_h);
    box.width = std::min(frame1.cols - box.x, box.width + 2 * padding_w);
    box.height = std::min(frame1.rows - box.y, box.height + 2 * padding_h);

    return box;
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