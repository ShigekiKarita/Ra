# CLion default
cmake_minimum_required(VERSION 2.8.4)
project(Ra)
set(SOURCE_FILES main.cpp)

set(CMAKE_CXX_COMPILER /usr/local/bin/clang++-3.5)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_FLAGS "-Wall -std=c++1z -stdlib=libc++ -U__STRICT_ANSI__ -DGTEST_USE_OWN_TR1_TUPLE=1")

set(GTEST_ROOT $ENV{HOME}/usr/src/gtest-1.7.0)
include_directories(
  ${GTEST_ROOT}/include
  src
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME} 
    pthread
    ${GTEST_ROOT}/build/libgtest.a
    ${GTEST_ROOT}/build/libgtest_main.a
    )


