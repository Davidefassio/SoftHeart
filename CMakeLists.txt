cmake_minimum_required(VERSION 3.12)

# set the project name
project(SoftHeart)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify compiler options only for release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        # On Windows the architecture must be set explicitly, if in doubt remove the flag /arch
        add_compile_options(/O2 /GL /arch:AVX2)
    else()
        add_compile_options(-O3 -flto -march=native)
    endif()
endif()

# add headers and executable
include_directories(${PROJECT_SOURCE_DIR})

add_executable(SoftHeart 
    src/main.cpp
    src/u3tp-simple.cpp
    src/Board.cpp
    src/Engine.cpp 
    src/Tree.cpp)
