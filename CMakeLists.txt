cmake_minimum_required(VERSION 3.10)
project(CV-MIDTERM-HOMEWORK)
include_directories(include)
set(SOURCES 
    src/main.cpp
    src/DataLoader.cpp
    src/feature_detection.cpp
    src/feature_matching.cpp
    src/box_definition.cpp
    src/IoU_calculation.cpp
)

find_package(OpenCV REQUIRED)
add_executable(test ${SOURCES})
target_link_libraries(test ${OpenCV_LIBS})

