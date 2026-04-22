#include "DataLoader.hpp"

    const string PATHIMAGES="../assets/data/";
    const string BIRD="bird";
    const string CAR="car";
    const string FROG="frog";
    const string SHEEP="sheep";
    const string SQUIRREL="squirrel";

Mat LoadImg(string folder,int number){
    string numberString=to_string(number);
    string zeros="/";
    Mat img;
    if(number>278){
        cout<<"Error number"<<endl;
        return Mat{};
    }
    for(int i=0;i<4-numberString.length();i++){
        zeros=zeros+"0";
    }
    if(folder!=CAR && folder!=BIRD && folder!=FROG && folder!=SHEEP && folder!=SQUIRREL){
        cout<<"Error folder"<<endl;
        return Mat{};
    }
    img=imread(PATHIMAGES+folder+zeros+numberString+".png");
    if(img.empty()){
        return imread(PATHIMAGES+folder+zeros+numberString+".jpg");
    }else{
        return img;
    }
}