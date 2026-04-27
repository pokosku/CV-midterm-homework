# CV Midterm Homework: Trinity Force group

## Prerequisites
Before building the project, ensure your system has the following installed:
* **CMake** (version 3.10 or higher)
* **OpenCV** (version 4.11)
* A standard C++ compiler (e.g., GCC, Clang)

## Build and Run Instructions

To ensure the executable runs correctly, the dataset must be placed in the correct directories, and an output folder must exist to collect the generated bounding boxes and images. 

Run the following commands from the root directory of the project. These commands will automatically set up the required directory structure, compile the code, and execute it.

```bash
# 1. Create the required dataset and output directories
mkdir -p assets output

# --- IMPORTANT ---
# 2. Before proceeding, manually place your dataset files into the assets folder:
#    - Put all sequence folders (e.g., bird, car) in: ./assets/
#    - Put the ground truth annotations (.txt) in: ./assets/

```
### Example project tree
```text
project/
├── build/                 
├── output/                # Generated bounding boxes and images
├── assets/               
│   ├── data/              # Image sequences 
│   └── labels/            # Ground truth annotations (.txt files)
├── CMakeLists.txt         # Build configuration
├── main.cpp               # Main source code

```
```bash
# 3. Create the build directory, configure, and compile the project
mkdir -p build
cd build
cmake ..
make

# 4. Run the executable
./test
