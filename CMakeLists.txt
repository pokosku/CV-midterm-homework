cmake_minimum_required(VERSION 3.10)
project(CV-MIDTERM-HOMEWORK)
include_directories(include)
set(SOURCES 
    src/main.cpp
    src/DataLoader.cpp
)

find_package(OpenCV REQUIRED)
add_executable(test ${SOURCES})
target_link_libraries(test ${OpenCV_LIBS})

