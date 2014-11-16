# CLion default
cmake_minimum_required(VERSION 2.8.4)
project(TMP)
set(SOURCE_FILES main.cpp)
#set(SOURCE_FILES vectorAdd.cu)

#find_package(CUDA REQUIRED)
#message("-- CUDA --")
#message("Version: " ${CUDA_VERSION})
#message("Library: " ${CUDA_CUDA_LIBRARY})
#message("Runtime: " ${CUDA_CUDART_LIBRARY})

add_definitions("-Wall")
add_definitions("-std=c++1y")

set(GTEST_ROOT $ENV{HOME}/usr/src/gtest-1.7.0)
include_directories(
    ${GTEST_ROOT}/include
    )

add_executable(TMP ${SOURCE_FILES})
#cuda_add_executable(TMP ${SOURCE_FILES})

target_link_libraries(TMP
    pthread
    ${GTEST_ROOT}/build/libgtest.a
    ${GTEST_ROOT}/build/libgtest_main.a
    )
