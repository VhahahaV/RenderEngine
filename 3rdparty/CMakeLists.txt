ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/argparse)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/glad)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/tinyply)
add_library(imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
add_executable(imgui-demo
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/example_glfw_opengl3/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
)

target_link_libraries(imgui-demo PUBLIC imgui glfw ${OPENGL_LIBRARIES} -ldl)


