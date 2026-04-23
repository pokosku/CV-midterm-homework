#include "DataLoader.hpp"

//constructor has to be fixed
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


// we can just use folder_images to access image path
cv::Mat DataLoader::load_test_img(){
    return cv::imread(folder_images[0]);
}


cv::Mat DataLoader::load_next_img(){

    if(index>number_images-1){
        std::cout<<"Out of index"<<std::endl;
        return cv::Mat{};
    }

    return cv::imread(folder_images[index++]);
    /*
    zeros="/";
    number_string=std::to_string(index);
    for(int i=0;i<4-number_string.length();i++){
        zeros=zeros+"0";
    }
    std::cout<< "reading file " << path+zeros+number_string+extension << std::endl;
    img=cv::imread(path+zeros+number_string+extension,cv::IMREAD_COLOR_BGR);

    
    index++;
    return img;
    */
}

cv::Mat DataLoader::load_next_img(int step){

    if(index>number_images){
        std::cout<<"Out of index"<<std::endl;
        return cv::Mat{};
    }

    cv::Mat img = cv::imread(folder_images[index]);
    index+=step;

    return img;
    /*
    zeros="/";
    number_string=std::to_string(index);
    for(int i=0;i<4-number_string.length();i++){
        zeros=zeros+"0";
    }
    std::cout<< "reading file " << path+zeros+number_string+extension << std::endl;
    img=cv::imread(path+zeros+number_string+extension);

    index+=step;
    return img;
    */
}