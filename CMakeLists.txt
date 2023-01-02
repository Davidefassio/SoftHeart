cmake_minimum_required(VERSION 3.12)

# set the project name
project(SoftHeart)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify compiler flags
set(CMAKE_CXX_FLAGS "-O2 -flto")

# add the executable
include_directories(${PROJECT_SOURCE_DIR})

add_executable(SoftHeart 
    src/main.cpp
    src/u3tp.cpp
    src/Board.cpp
    src/Engine.cpp)