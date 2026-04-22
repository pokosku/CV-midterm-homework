#include "DataLoader.hpp"
use namespace std;
use namespace cv;

Mat LoadImg(string folder,int number){
    const string PATHIMAGES="../assets/data/";
    const string BIRD="bird";
    const string CAR="car";
    const string FROG="frog";
    const string SHEEP="sheep";
    const string SQUIRREL="squirrel";
    
    for(int i=0;i<number;i++){
        Mat img =imread(PATHIMAGES+folder+"/000"+to_string(i)+".png");
        if(img.empty()){
            cout << "Error : could not load image" <<endl;
            return -1;
        }
       
    }
    return 0;
}