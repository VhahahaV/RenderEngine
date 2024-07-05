cmake_minimum_required(VERSION 3.28)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Vulkan REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

ADD_EXECUTABLE(VulkanExample vulkan_example.cpp)
target_link_libraries(VulkanExample PUBLIC glm::glm glad glfw ${OpenCV_LIBS} ${Vulkan_LIBRARIES} utils Camera imgui)
