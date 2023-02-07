# Blogs and sources for [GenZCpp](https://genzcpp.com/)

## Build Instructions
### Prerequisites
- [cmake build only] Requires `boost 1.80.0`. For MacOS, just `brew install boost`
- [cmake build only] Requires `cmake 3.20.0`. For MacOS, just `brew install cmake`
- Requires `GNC C++` compiler `g++-12 (Homebrew GCC 12.2.0) 12.2.0`. 
    - For MacOS, just `brew install gcc` 
    - then add `alias g++12="/opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12` to `~/.zprofile`.

### gcc build
`./gcc_build.sh ARG1.cpp` This script compiles and run the executable for `ARG1.cpp`


### cmake build (boost required)
`./cmake_build.sh ARG1` This script compiles and run the executable `ARG1.cpp`

> For example, to build `auto_type_deduction.cpp`, just do

`./cmake_build.sh auto_type_deduction`

