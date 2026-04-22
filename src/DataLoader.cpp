#include "DataLoader.hpp"

DataLoader::DataLoader(std::string directory){
    folder=directory;
    path ="";
    number_images = 0;
    number_string = "";
    zeros = "";
    index = 0;
    extension = "";

    if(folder!=CAR && folder!=BIRD && folder!=FROG && folder!=SHEEP && folder!=SQUIRREL){
        std::cout<<"Error folder"<<std::endl;
    }else{
        path=PATHIMAGES+folder;
        
        cv::glob(path, folder_images, false);
        number_images = folder_images.size();
        // glob returns a list of the files in the folder, we can exploit it for obtaining the extension
        extension = folder_images[0].substr(folder_images[0].length()-4, 4);
    }
}


cv::Mat DataLoader::load_test_img(){
    return cv::imread(path+"/0000"+extension);
}

cv::Mat DataLoader::load_next_img(){

    if(index>number_images){
        std::cout<<"Out of index"<<std::endl;
        return cv::Mat{};
    }
    zeros="/";
    number_string=std::to_string(index);
    for(int i=0;i<4-number_string.length();i++){
        zeros=zeros+"0";
    }
    std::cout<< "reading file " << path+zeros+number_string+extension << std::endl;
    img=cv::imread(path+zeros+number_string+extension);

    index++;
    return img;

}