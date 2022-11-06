echo building "$1"
rm -rf build
mkdir build
cd build
cmake -S .. -B . -Dfilename=$1
cmake --build .
./main