if(WIN32)
    set(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")
    set(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")
elseif(UNIX)
    set(CMAKE_C_COMPILER "/usr/bin/cc")
    set(CMAKE_CXX_COMPILER "/usr/bin/c++")
endif()


