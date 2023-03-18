cmake_minimum_required(VERSION 3.12)

# set the project name
project(SoftHeart)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify compiler options
# TODO: test use of fast fp math (/fp:fast , --ffast-math)
if (MSVC)
    add_compile_options(/O2)
else()
    add_compile_options(-O3)
endif()

# add headers and executable
include_directories(${PROJECT_SOURCE_DIR})

add_executable(SoftHeart 
    src/main.cpp
    src/u3tp-simple.cpp
    src/Board.cpp
    src/Engine.cpp 
    src/Tree.cpp)
