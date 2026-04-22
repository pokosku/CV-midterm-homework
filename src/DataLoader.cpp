#include "DataLoader.hpp"

Mat LoadImg(string folder,int number){
    const string PATHIMAGES="../assets/data/";
    const string BIRD="bird";
    const string CAR="car";
    const string FROG="frog";
    const string SHEEP="sheep";
    const string SQUIRREL="squirrel";
    return imread(PATHIMAGES+folder+"/000"+to_string(number)+".png");
}