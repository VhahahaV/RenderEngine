cmake_minimum_required(VERSION 3.28)

set(GUI_SOURCE Gui.cpp Gui.h ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/imgui/imgui_demo.cpp)
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

find_package(glfw3 REQUIRED)

add_library(gui ${GUI_SOURCE})
target_link_libraries(gui PUBLIC imgui glfw utils Camera Render ${OPENGL_LIBRARIES} -ldl)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test test.cpp)
target_link_libraries(test PRIVATE PUBLIC imgui glfw ${OPENGL_LIBRARIES} ${Vulkan_LIBRARIES} -ldl)
