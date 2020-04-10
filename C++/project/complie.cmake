if(WIN32)
    set(CMAKE_C_COMPILER "gcc.exe")
    set(CMAKE_CXX_COMPILER "g++.exe")
elseif(UNIX)
    set(CMAKE_C_COMPILER "/usr/bin/cc")
    set(CMAKE_CXX_COMPILER "/usr/bin/c++")
endif()


