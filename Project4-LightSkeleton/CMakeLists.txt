cmake_minimum_required(VERSION 3.10)
project(Project4_LightSkeleton)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project4_LightSkeleton
        main.cpp
        StudentTextEditor.cpp
        StudentTextEditor.h)
