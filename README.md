# CV Midterm Homework: Trinity Force group

## Prerequisites
Before building the project, ensure your system has the following installed:
* **CMake** (version 3.10 or higher)
* **OpenCV** (configured and accessible in your environment)
* A standard C++ compiler (e.g., GCC, Clang)

## Build Instructions
1. **Dataset**: be sure to keep the same structure when donwloading the dataset. Having a directory "assets" in the project with inside the directories "labels" and "data".

2. **Output folder** to collect output images and corners of the rectangle:
Verify that exists an output directory and be sure to be in the build folder to run the cmake.
Follow instructions below while located in the project folder.

3. **Create the required directories** for the build files and the generated outputs:
```bash
mkdir output build
cd build
cmake ..
make
./test
```

