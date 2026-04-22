#include "DataLoader.hpp"

DataLoader::DataLoader(string directory){
    folder=directory;
    if(folder!=CAR && folder!=BIRD && folder!=FROG && folder!=SHEEP && folder!=SQUIRREL){
        cout<<"Error folder"<<endl;
    }else{
        path=PATHIMAGES+folder;
        glob(path, folder_images, false);
        number_images= folder_images.size();
    }
}

Mat DataLoader::LoadFirst(){
    img=imread(path+"/0001"+".png");
      if(!img.empty()){
        extension=".png";
        index=1;
        return img;
    }else{
        img=imread(path+zeros+number_string+".jpg");
        if(!img.empty()){
            extension=".jpg";
            index=1;
            return img;
        }else{
            cout<<"Error no matching img"<<endl;
            return Mat{};
        }        
    }
}

Mat DataLoader::LoadNext(){
    index=index+1;
    if(index>number_images){
        cout<<"Out of index"<<endl;
        return Mat{};
    }
    zeros="/";
    number_string=to_string(index);
    for(int i=0;i<4-number_string.length();i++){
        zeros=zeros+"0";
    }
    img=imread(path+zeros+number_string+extension);
    return img;

}