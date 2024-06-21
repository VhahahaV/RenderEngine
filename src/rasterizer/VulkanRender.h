//
// Created by czq on 24-6-8.
//

#ifndef VULKANRENDER_H
#define VULKANRENDER_H

#include "RenderBase.h"

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
public:
    static void windowSizeCallback(GLFWwindow*, int, int);
};

class VulkanRender final : public RenderBase
{
private:
    RENDER_TYPE mType = RENDER_TYPE::Vulkan;
    std::shared_ptr<VulkanContextManager> mVulkanContext{};

public:

    ~VulkanRender() override;
    void init() override;
    void render(std::shared_ptr<CameraBase> camera, const Resolution& resolution, ImDrawData* draw_data) override;
    void loadModel(std::unique_ptr<Model> model) override;
    [[nodiscard]] RENDER_TYPE getType() const override;

    void FrameRender(ImGui_ImplVulkanH_Window* wd,ImDrawData* draw_data);
    void cleanup() override;

    void FramePresent(ImGui_ImplVulkanH_Window* wd);

};





#endif //VULKANRENDER_H
