#include "IoU_calculation.hpp"

const std::string PATHIMAGES="../assets/labels/";
const std::string BIRD="bird";
const std::string CAR="car";
const std::string FROG="frog";
const std::string SHEEP="sheep";
const std::string SQUIRREL="squirrel";
float calculateIoU(cv::Rect rectA,std::string folder) {
    std::string path="";
    float x1,y1,x2,y2;
    //check for path
    if(folder!=CAR && folder!=BIRD && folder!=FROG && folder!=SHEEP && folder!=SQUIRREL){
        std::cout<<"Error folder"<<std::endl;
        return -1;
    }else{
        path=PATHIMAGES+folder+"/0000.txt";
        //reading label
        std::ifstream file(path);
        //error if label is open
        if(!file.is_open()) {
            std::cerr << "Error opening file" << std::endl;
            return 1;
        }
        //reading label
        while(file >> x1 >> y1 >> x2 >> y2);

        //creating rect with labels found
        cv::Rect rectB(x1, y1, x2,y2);
        cv::Rect intersection = rectA & rectB;

        // Calculating areas
        float areaInt = static_cast<float>(intersection.area());
        float areaA = static_cast<float>(rectA.area());
        float areaB = static_cast<float>(rectB.area());
        float areaUnion = areaA + areaB - areaInt;
        
        //boh gemini dice di metterlo ma non so
        if (areaUnion <= 0.0f) {
            return -1;
        }
        
        return areaInt / areaUnion;
    }
    
}
