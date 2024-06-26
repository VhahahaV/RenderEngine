cmake_minimum_required(VERSION 3.28)
set(RENDER_SRC RenderBase.h OpenglRender.cpp OpenglRender.h VulkanRender.cpp VulkanRender.h)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Vulkan REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(Render ${RENDER_SRC})
target_link_libraries(Render PUBLIC glm::glm glad glfw ${OpenCV_LIBS} ${Vulkan_LIBRARIES} utils Camera imgui)
target_include_directories(Render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE(VulkanExample vulkan_example.cpp)
target_link_libraries(VulkanExample PUBLIC glm::glm glad glfw ${OpenCV_LIBS} ${Vulkan_LIBRARIES} utils Camera imgui)
