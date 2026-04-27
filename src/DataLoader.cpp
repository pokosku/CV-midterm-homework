// AUTHORS: Daniele Riolmi Rossetto, Leonardo Joao Fabbro

#include "DataLoader.hpp"

DataLoader::DataLoader(){
    folder="";
    path ="";
    number_images = 0;
    number_string = "";
    zeros = "";
    index = 0;
    extension = "";
}
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
        std::cerr<<"Error folder"<<std::endl;
    }else{
        path=PATHIMAGES+folder;

        // Verifying presence of assets folder
        try{
            cv::glob(path, folder_images, false);
        }catch(const std::exception& e){
            throw std::runtime_error("Error missing folder. Checkout if directory assets exists!!");
        }

        number_images = folder_images.size();
        // glob returns a list of the files in the folder, we can exploit it for obtaining the extension
        if (!folder_images.empty()) {
            extension = folder_images[0].substr(folder_images[0].length()-4, 4);
        } else {
            throw std::runtime_error("Error missing folder structure. Checkout if dataset is loaded correctly!!");
        }
    }
}

// we can just use folder_images to access image path
cv::Mat DataLoader::load_test_img(){
    if (folder_images.empty()) {
        return cv::Mat{};
    }
    return cv::imread(folder_images[0]);
}


cv::Mat DataLoader::load_next_img(){

    if(index>=number_images){
        //std::cout<<"Out of index"<<std::endl;
        return cv::Mat{};
    }
    return cv::imread(folder_images[index++]);
}

cv::Mat DataLoader::load_next_img(int step){

    if(index>=number_images){
        //std::cout<<"Out of index"<<std::endl;
        return cv::Mat{};
    }

    cv::Mat img = cv::imread(folder_images[index]);
    index+=step;

    return img;
}

void DataLoader::reset_head(){
    index = 0;
}

std::size_t DataLoader::get_number_images(){
    return number_images;
}