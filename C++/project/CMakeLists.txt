include(complie.cmake)

cmake_minimum_required(VERSION 3.0)

project(learn_cpp)

#告警可以加-Wall -Wno去掉某些告警
set(CMAKE_C_FLAGS "-g -O0 -Wall -std=c11 ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wno-unused-variable -std=c++11 -pthread ${CMAKE_CXX_FLAGS}")

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(CODE_DIR "${ROOT_DIR}/code")
set(TEST_DIR "${ROOT_DIR}/test")

include_directories(${TEST_DIR}
                    ${CODE_DIR})

file(GLOB files "${CODE_DIR}/*.cpp"
                "${TEST_DIR}/*.cpp")

add_executable(a.out ${files})
