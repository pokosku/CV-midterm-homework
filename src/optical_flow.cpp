#include "optical_flow.hpp"

// Traccia i punti dal primo all'ultimo frame e restituisce quanto si è mosso ogni punto
std::vector<float> track_features(const std::vector<cv::Mat>& frames, const std::vector<cv::Point2f>& p0) {
    std::vector<cv::Point2f> p_current = p0;
    std::vector<float> total_displacements(p0.size(), 0.0f);
    
    cv::Mat gray_prev, gray_curr;
    cv::cvtColor(frames[0], gray_prev, cv::COLOR_BGR2GRAY);

    for (size_t i = 1; i < frames.size(); ++i) {
        cv::cvtColor(frames[i], gray_curr, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point2f> p_next;
        std::vector<uchar> status;
        std::vector<float> err;

        cv::calcOpticalFlowPyrLK(gray_prev, gray_curr, p_current, p_next, status, err);

        for (size_t j = 0; j < p0.size(); ++j) {
                if (status[j]) {
                    total_displacements[j] = (float)cv::norm(p_next[j] - p0[j]);
                    p_current[j] = p_next[j]; 
                } else {
                    total_displacements[j] = -1.0f;
                }
        }
        gray_prev = gray_curr.clone();
    }
    return total_displacements;
}

// Filtra i punti iniziali usando Otsu sugli spostamenti
std::vector<cv::Point2f> filter_moving_points(const std::vector<cv::Point2f>& p0, 
                                              const std::vector<float>& displacements, 
                                              cv::Size img_size) {
    cv::Mat heatmap = cv::Mat::zeros(img_size, CV_8UC1);
    
    // Trova il movimento massimo per normalizzare
    float max_d = 0;
    for (float d : displacements) if (d > max_d) max_d = d;
    if (max_d < 2.0f) return {}; // Se tutto si muove meno di 2 pixel, è solo rumore

    for (size_t i = 0; i < p0.size(); ++i) {
        // FILTRO ANTIRUMORE: ignora chi si è mosso meno di 1.5 pixel in totale
        if (displacements[i] < 1.5f) continue; 

        uchar val = static_cast<uchar>((displacements[i] / max_d) * 255);
        // Usa un raggio più piccolo (3 invece di 5) per non "gonfiare" troppo la box
        cv::circle(heatmap, p0[i], 3, cv::Scalar(val), -1);
    }

    cv::Mat mask;
    cv::threshold(heatmap, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // MORFOLOGIA: Fondamentale per CAR e SQUIRREL
    // L'apertura toglie i puntini isolati (rumore sullo sfondo)
    // La dilatazione unisce i punti dell'oggetto (es. ruote e carrozzeria della car)
    cv::Mat kernel_small = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat kernel_big = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
    
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel_small);
    cv::dilate(mask, mask, kernel_big); 

    std::vector<cv::Point2f> object_points;
    for (size_t i = 0; i < p0.size(); ++i) {
        cv::Point pt(cvRound(p0[i].x), cvRound(p0[i].y));
        if (pt.x >= 0 && pt.x < img_size.width && pt.y >= 0 && pt.y < img_size.height) {
            if (mask.at<uchar>(pt) > 0) {
                object_points.push_back(p0[i]);
            }
        }
    }
    return object_points;
}