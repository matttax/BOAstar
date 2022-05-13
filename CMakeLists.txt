cmake_minimum_required(VERSION 3.20)
project(BOAstar)

set(CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(BOAstar Src/PathPlanning.cpp Src/Node.h Src/Search.cpp Src/Search.h Src/Open.cpp Src/Open.h Src/Map.cpp Src/Map.h)
