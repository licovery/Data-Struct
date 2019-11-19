#!/bin/bash

#windows
cd project
rm -rf build/
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ..  
make
./a.exe
