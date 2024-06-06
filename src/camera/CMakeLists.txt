
SET(CAMERA_SCR CameraBase.h PerspectiveCamera.cpp PerspectiveCamera.h)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
add_library(Camera ${CAMERA_SCR})
target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Camera PUBLIC glm::glm utils)