ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/argparse)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/glad)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/stbimg)

add_library(imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(imgui-demo
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/example_glfw_opengl3/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
)
target_link_libraries(imgui-demo PUBLIC imgui glfw ${OPENGL_LIBRARIES}  -ldl)

add_executable(vulkan-demo
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/example_glfw_vulkan/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp
)
target_link_libraries(vulkan-demo PUBLIC imgui glfw ${OPENGL_LIBRARIES} ${Vulkan_LIBRARIES} -ldl)
