#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

#include "DataLoader.hpp"
#include "feature_detection.hpp"
#include "feature_matching.hpp"
#include "optical_flow.hpp"
#include "box_definition.hpp"
#include "IoU_calculation.hpp"


int main(int argc, char** argv) {
    // 1. Definizione delle classi da analizzare
    std::vector<std::string> classes = {"bird", "car", "frog", "sheep", "squirrel"};
    std::string win="win";
    float total_iou = 0.0f;
    int correct_predictions = 0;
    int total_classes = static_cast<int>(classes.size());

    std::cout << "--- ANALISI PERFORMANCE TRACKING ---" << std::endl;
    std::cout << std::left << std::setw(12) << "CLASSE" 
              << std::setw(10) << "IoU" 
              << "ESITO (IoU > 0.5)" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;

    for (const std::string& folder : classes) {
        // 2. Caricamento dei frame per la classe corrente
        DataLoader dl(folder);
        std::vector<cv::Mat> frames;

        cv::Mat frame_t = dl.load_next_img();
        while (!frame_t.empty()) {
            frames.push_back(frame_t.clone());
            frame_t = dl.load_next_img();
        }

        if (frames.empty()) {
            std::cerr << "Errore: nessun frame trovato per " << folder << std::endl;
            continue;
        }

        int interval = frames.size() /20;
        if(interval <2){
            interval=2;
        }
        cv::Mat frame_start = frames[0];
        cv::Mat frame_end = frames[interval];

        cv::Mat motion_mask = get_refined_motion_mask(frame_start, frame_end);

        cv::Rect predicted_box = get_smart_bbox(motion_mask);

        float current_iou = calculate_IoU(predicted_box, folder);
        
        // Se calculate_IoU restituisce -1 (errore), lo gestiamo come 0 per le statistiche
        float valid_iou = (current_iou < 0) ? 0.0f : current_iou;
        total_iou += valid_iou;

        bool is_accurate = (valid_iou > 0.5f);
        if (is_accurate) {
            correct_predictions++;
        }

        // 5. Output riga per classe
        std::cout << std::left << std::setw(12) << folder 
                  << std::setw(10) << std::fixed << std::setprecision(3) << valid_iou 
                  << (is_accurate ? "[OK]" : "[FAIL]") << std::endl;
        
        // Opzionale: Mostra il risultato a video per ogni classe
        
        cv::Mat display = frames[0].clone();
        cv::rectangle(display, predicted_box, cv::Scalar(0, 255, 0), 2);
        cv::namedWindow(win+folder,cv::WINDOW_NORMAL);
        cv::imshow(win+folder, display);
        cv::waitKey(0); // Mezzo secondo di pausa tra le classi
        
    }

    // 6. Calcolo e stampa risultati finali
    float mean_iou = total_iou / total_classes;
    float accuracy = (static_cast<float>(correct_predictions) / total_classes) * 100.0f;

    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "IoU MEDIA:    " << std::fixed << std::setprecision(3) << mean_iou << std::endl;
    std::cout << "ACCURACY:     " << std::fixed << std::setprecision(2) << accuracy << "%" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;

    return 0;
}