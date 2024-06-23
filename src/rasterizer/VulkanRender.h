//
// Created by czq on 24-6-8.
//

#ifndef VULKANRENDER_H
#define VULKANRENDER_H

#include "RenderBase.h"
#include <vector>
//https://frguthmann.github.io/posts/vulkan_imgui/
// https://cloud.tencent.com/developer/ask/sof/106785584
class VulkanContextManager: public ContextManager
{
    friend class VulkanRender;
protected:
    ImGui_ImplVulkanH_Window mMainWindowData{};
    VkAllocationCallbacks* mAllocator = nullptr;
    VkInstance mInstance = VK_NULL_HANDLE;
    VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
    VkDevice mDevice = VK_NULL_HANDLE;
    uint32_t mQueueFamily = (uint32_t)-1;
    VkQueue mQueue = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT mDebugReport = VK_NULL_HANDLE;
    VkPipelineCache mPipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;

    int mMinImageCount = 2;
    bool mSwapChainRebuild = false;


    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
public:
    GLFWwindow* makeContext() override;
    void SetupVulkan(ImVector<const char*> instance_extensions);
    static void check_vk_result(VkResult err);
    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    void SetupVulkanWindow(VkSurfaceKHR surface, int width, int height);
    void setupPlatform(GLFWwindow* window) override;
    void cleanup(GLFWwindow*) override;
    void init(GLFWwindow* window) override;
    static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
    void render(ImDrawData* draw_data) override;
    void setCallback(GLFWwindow* window) override;
    void resizeWindowSize(GLFWwindow* window) override;

    void createVertexBuffer(const std::shared_ptr<Model>& model);
    void createIndexBuffer(const std::shared_ptr<Model>& model);
    void createDescriptorSetLayout();
    void createUniformBuffer();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                  VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)

public:
    static void windowSizeCallback(GLFWwindow*, int, int);
};

class VulkanRender final : public RenderBase
{
private:
    RENDER_TYPE mType = RENDER_TYPE::Vulkan;
    std::unique_ptr<Model> mModel;


public:

    ~VulkanRender() override;
    void init() override;
    void render(std::shared_ptr<CameraBase> camera, const Resolution& resolution) override;
    void loadModel(std::unique_ptr<Model> model) override;
    [[nodiscard]] RENDER_TYPE getType() const override;

    void cleanup() override;



};





#endif //VULKANRENDER_H
