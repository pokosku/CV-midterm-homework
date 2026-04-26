// AUTHORS: Daniele Riolmi Rossetto, Zavarise Mirco

#include "IoU_calculation.hpp"

const std::string PATHIMAGES="../assets/labels/";
const std::string BIRD="bird";
const std::string CAR="car";
const std::string FROG="frog";
const std::string SHEEP="sheep";
const std::string SQUIRREL="squirrel";

float calculate_IoU(cv::Rect rect_a,std::string folder) {
    std::string path="";
    float x1,y1,x2,y2;
    // Checking for path integrity
    if(folder!=CAR && folder!=BIRD && folder!=FROG && folder!=SHEEP && folder!=SQUIRREL){
        std::cout<<"Error folder"<<std::endl;
        return -1;
    }

    // Selecting single squirrel label
    if(folder== SQUIRREL){
        path=PATHIMAGES+folder+"/single_squirrel/0000.txt";
    }
    else{
        path=PATHIMAGES+folder+"/0000.txt";
    }
    // Reading label
    std::ifstream file(path);

    // Checking if the file is open
    if(!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    while(file >> x1 >> y1 >> x2 >> y2);

    // Creating rect with label found
    float width = x2 - x1;
    float height = y2 - y1;
    cv::Rect rect_b(x1, y1, width, height);
    cv::Rect intersection = rect_a & rect_b;

    // Calculating areas
    float area_int = static_cast<float>(intersection.area());
    float area_a = static_cast<float>(rect_a.area());
    float area_b = static_cast<float>(rect_b.area());
    float area_union = area_a + area_b - area_int;
    
    // Checking division by 0
    if (area_union <= 0.0f) {
        return -1;
    }
    
    // Return IoU
    return area_int / area_union;


}

void generate_output(cv::Rect rect_a, cv::Mat frame, std::string method, std::string class_name){

    // Cheking if string method is correct
    if(method!="SIFT" && method!="optical_flow"){
        std::cerr<<"Wrong method"<<std::endl;
        return;
    }

    // Drawing bounding box on frame
    cv::Mat out= frame.clone();
    cv::rectangle(out,rect_a,cv::Scalar(0,0,255),2);

    // Defining image path
    std::string filename_img="../output/"+method+"_"+class_name+".jpg";
    // Saving image
    cv::imwrite(filename_img,out);

    // Defining bounding box path
    std::string filename_bbox = "../output/"+method+"_"+class_name+".txt";
    std::ofstream file(filename_bbox);

    // Checking if file is open
    if(!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }  

    // Writing coordinates on txt file
    file<<rect_a.tl().x<<" "<<rect_a.tl().y<<" "<<rect_a.br().x<<" "<<rect_a.br().y;

}
