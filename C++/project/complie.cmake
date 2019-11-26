set(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")

#set(CMAKE_C_COMPILER "C:/TDM-GCC-64/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/TDM-GCC-64/bin/g++.exe")

#告警可以加-Wall -Wno去掉某些告警
set(CMAKE_C_FLAGS "-g -O0 -Wall -std=c11 ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wno-unused-variable -std=c++11 ${CMAKE_CXX_FLAGS}")
