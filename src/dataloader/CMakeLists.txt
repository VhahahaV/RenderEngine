cmake_minimum_required(VERSION 3.28)

SET(DATASET_SRC jsonParser.cpp DatasetBase.h)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(DataSet ${DATASET_SRC})
target_link_libraries(DataSet  ${OpenCV_LIBS})
target_include_directories(DataSet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})