cmake_minimum_required(VERSION 3.12)

# set the project name
project(SoftHeart)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
include_directories(${PROJECT_SOURCE_DIR})

add_executable(SoftHeart 
    example/UltimateTTT.cpp
    example/Game.cpp
    src/Board.cpp
    src/Engine.cpp)