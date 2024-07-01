//
// Created by czq on 24-6-8.
//

#ifndef VULKANRENDER_H
#define VULKANRENDER_H

#include "RenderBase.h"
#include <vector>

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
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
    VkRenderPass RenderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    // std::vector<VkFramebuffer> swapChainFramebuffers;
    VkDescriptorSet descriptorSet;

    int mMinImageCount = 2;
    bool mSwapChainRebuild = false;
    VkQueue graphicsQueue;

    // VkSwapchainKHR swapChain;
    // std::vector<VkImage> swapChainImages;
    // VkFormat swapChainImageFormat;
    // VkExtent2D swapChainExtent;

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

    void createDescriptorPool();
    void createDescriptorSet();
    void createVertexBuffer(const std::shared_ptr<Model>& model);
    void createIndexBuffer(const std::shared_ptr<Model>& model);
    void createDescriptorSetLayout();
    void createUniformBuffer();
    void createCommandBuffers(const std::shared_ptr<Model>& mModel);
    void createRenderPass();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                  VkDeviceMemory& bufferMemory);
    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    static std::vector<char> readFile(const std::string& filename);
    VkFormat findDepthFormat();
    void updateUniformBuffer(glm::mat4 modelMatrix , glm::mat4 viewMatrix,glm::mat4 projectMatrix);


public:
    static void windowSizeCallback(GLFWwindow*, int, int);
};

class VulkanRender final : public RenderBase
{
private:
    RENDER_TYPE mType = RENDER_TYPE::Vulkan;
    std::shared_ptr<Model> mModel;
    std::shared_ptr<VulkanContextManager> mManager;

public:

    ~VulkanRender() override;
    void init() override;
    void render(std::shared_ptr<CameraBase> camera, const Resolution& resolution) override;
    void loadModel(std::shared_ptr<Model> model) override;
    [[nodiscard]] RENDER_TYPE getType() const override;
    void loadManager(std::shared_ptr<VulkanContextManager> manager);
    void cleanup() override;
    void loadManager(std::shared_ptr<ContextManager> manager) override;



};





#endif //VULKANRENDER_H