cmake_minimum_required(VERSION 3.10)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project2 testSequence.cpp Sequence.cpp Sequence.h ScoreList.cpp ScoreList.h)
