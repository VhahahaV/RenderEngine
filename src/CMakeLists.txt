cmake_minimum_required(VERSION 3.28)
SET(CMAKE_CXX_STANDARD 17)

project(SCR_CODE)
add_subdirectory(camera)
add_subdirectory(dataset)
add_subdirectory(imgui)
add_subdirectory(rasterizer)
add_subdirectory(utils)