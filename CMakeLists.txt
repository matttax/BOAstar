cmake_minimum_required(VERSION 3.20)
project(BOAstar)

set(CMAKE_CXX_STANDARD 23)

add_executable(BOAstar main.cpp Node.h Search.cpp Search.h Open.cpp Open.h Map.cpp Map.h)
