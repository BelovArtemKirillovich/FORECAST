rm -rf build
mkdir build
cd build
cmake ..
cd ../
cmake --build .
./app/app_main