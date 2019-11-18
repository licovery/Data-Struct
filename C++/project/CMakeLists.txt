cmake_minimum_required(VERSION 3.0)

project(learn_cpp)

include(complie.cmake)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(CODE_DIR "${ROOT_DIR}/code")
set(TEST_DIR "${ROOT_DIR}/test")

include_directories(${TEST_DIR}
                    ${CODE_DIR})

file(GLOB files "${CODE_DIR}/*.cpp"
                "${TEST_DIR}/*.cpp")

add_executable(a ${files})