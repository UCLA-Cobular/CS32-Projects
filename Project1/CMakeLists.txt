cmake_minimum_required(VERSION 2.8.12.2)
project(Project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project1 main.cpp utilities.cpp globals.h Flatulan.cpp Flatulan.h Player.cpp Player.h City.cpp City.h Game.cpp Game.h History.cpp History.h)
