# Copyright (c) 2020 smarsufan. All Rights Reserved.

cmake_minimum_required(VERSION 3.6.0)
project(smcc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -Wall -Werror")

include_directories(core)

add_subdirectory(core)
add_subdirectory(tests)
