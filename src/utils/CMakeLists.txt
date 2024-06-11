cmake_minimum_required(VERSION 3.28)
SET(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(glm REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(utils
        Model.cpp
        Model.h
        CameraUtils.cpp
        CameraUtils.h
        ParseStruct.h
        debugFuncs.hpp)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(utils PUBLIC tinyobjloader tinyply  glm::glm ${OpenCV_LIBS})