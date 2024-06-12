//
// Created by czq on 24-6-8.
//

#ifndef VULKANRENDER_H
#define VULKANRENDER_H

#include "RenderBase.h"

//https://frguthmann.github.io/posts/vulkan_imgui/
// https://cloud.tencent.com/developer/ask/sof/106785584
class VulkanRender : RenderBase
{
private:
    RENDER_TYPE mType = RENDER_TYPE::Vulkan;
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
    ~VulkanRender() override;
    void setupPlatform(ImGui_ImplVulkanH_Window* wd)  override;
    void initRender() override;
    void render(std::shared_ptr<CameraBase> camera, const Resolution& resolution) override;
    void cleanup(ImGui_ImplVulkanH_Window* wd) override;
    void loadModel(std::unique_ptr<Model> model) override;
    static void check_vk_result(VkResult err);
    [[nodiscard]] RENDER_TYPE getType() const override;

    static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    void SetupVulkan(ImVector<const char*> instance_extensions);
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd,VkSurfaceKHR surface, int width, int height);
    void FrameRender(ImGui_ImplVulkanH_Window* wd,ImDrawData* draw_data);

    void FramePresent(ImGui_ImplVulkanH_Window* wd);

};





#endif //VULKANRENDER_H
