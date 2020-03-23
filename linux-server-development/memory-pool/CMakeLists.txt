cmake_minimum_required(VERSION 3.0.0)
project(mempool VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "-g -Wall -Wno-sign-compare  -std=c++11  ${CMAKE_CXX_FLAGS}")

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CODE_DIR "${ROOT_DIR}/code")
set(TEST_DIR "${ROOT_DIR}/test")

include_directories(${TEST_DIR}
                    ${CODE_DIR})
                
file(GLOB files "${CODE_DIR}/*.cpp"
                "${TEST_DIR}/*.cpp")

add_executable(mempool ${files})
target_link_libraries(mempool -lgtest -lpthread)

