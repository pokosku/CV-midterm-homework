#include "optical_flow.hpp"

// Funzione che prende due frame e restituisce la maschera binaria del movimento
cv::Mat get_refined_motion_mask(const cv::Mat& frame1, const cv::Mat& frame2) {
 cv::Mat gray1, gray2;
    
    // 1. Pre-Processing
    cv::cvtColor(frame1, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(frame2, gray2, cv::COLOR_BGR2GRAY);
    
    // Un leggero blur uccide il rumore di fondo
    cv::GaussianBlur(gray1, gray1, cv::Size(3, 3), 0);
    cv::GaussianBlur(gray2, gray2, cv::Size(3, 3), 0);

    // 2. Calcolo Optical Flow Denso (Farneback)
    cv::Mat flow;
    // Parametri classici bilanciati per precisione e velocità
    cv::calcOpticalFlowFarneback(gray1, gray2, flow, 0.3, 3, 12, 3, 3, 1.1, 0);

    // 3. Separazione canali (X, Y) e calcolo Magnitudo
    cv::Mat flow_parts[2];
    cv::split(flow, flow_parts); // flow_parts[0] = X, flow_parts[1] = Y
    
    cv::Mat magnitude, angle;
    cv::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);

    // [Controllo di sicurezza]: Se non si muove niente, restituisci maschera vuota
    double min_val, max_val;
    cv::minMaxLoc(magnitude, &min_val, &max_val);
    if (max_val < 1.0) { // Nessun pixel si è mosso più di 1 pixel
        return cv::Mat::zeros(gray1.size(), CV_8U);
    }

    // 4. Segmentazione (Creazione Maschera)
    cv::Mat mag_norm, mask;
    // Portiamo i valori nel range 0-255 per poter usare Otsu
    cv::normalize(magnitude, mag_norm, 0, 255, cv::NORM_MINMAX);
    mag_norm.convertTo(mag_norm, CV_8U);
    
    // Applichiamo Otsu per trovare automaticamente la soglia ideale
    cv::threshold(mag_norm, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // 5. Pulizia Morfologica
    cv::Mat kOpen = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::Mat kClose = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 21));
    
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kOpen);   // Toglie i puntini di rumore
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kClose); // Compatta l'oggetto (es. unisce testa e coda)

    return mask;
}
