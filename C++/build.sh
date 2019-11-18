#!/bin/bash

#windows
cd project
rm -rf build/
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
./a.exe
