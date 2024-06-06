cmake_minimum_required(VERSION 3.28)

find_package(OpenCV REQUIRED)
find_package(RapidJSON REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(utils Mesh.cpp Mesh.h Parser.cpp Parser.h)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(utils PUBLIC tinyobjloader tinyply RapidJSON ${OpenCV_LIBS})