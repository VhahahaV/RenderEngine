//
// Created by czq on 24-6-8.
//

#include "VulkanRender.h"

#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

GLFWwindow* VulkanContextManager::makeContext()
{
    if (!glfwInit())
        throw std::runtime_error("glfw init error");

    // Create window with Vulkan contextt
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(
        1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
    if (!glfwVulkanSupported())
    {
        throw std::runtime_error("GLFW: Vulkan Not Supported\n");
    }

    ImVector<const char*> extensions;
    uint32_t extensions_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    for (uint32_t i = 0; i < extensions_count; i++)
        extensions.push_back(glfw_extensions[i]);
    SetupVulkan(extensions);
    glfwSetWindowUserPointer(window, this);
    return window;
}

VulkanRender::~VulkanRender()
= default;

void VulkanContextManager::setupPlatform(GLFWwindow* window)
{
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = mInstance;
    init_info.PhysicalDevice = mPhysicalDevice;
    init_info.Device = mDevice;
    init_info.QueueFamily = mQueueFamily;
    init_info.Queue = mQueue;
    init_info.PipelineCache = mPipelineCache;
    init_info.DescriptorPool = mDescriptorPool;
    init_info.RenderPass = mMainWindowData.RenderPass;
    init_info.Subpass = 0;
    init_info.MinImageCount = mMinImageCount;
    init_info.ImageCount = mMainWindowData.ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = mAllocator;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);
}

void VulkanRender::init()
{
    // TODO
}

void VulkanRender::render(std::shared_ptr<CameraBase> camera, const Resolution& resolution)
{
    // TODO
    // 1. pass uniform(e.g. MVP)

    // void* data;
    // vkMapMemory(device, uniformBufferMemory, 0, sizeof(glm::mat4) * 3, 0, &data);
    // memcpy(data, &ubo, sizeof(ubo));
    // vkUnmapMemory(device, uniformBufferMemory);
}

// void VulkanRender::render(std::shared_ptr<CameraBase> camera, const Resolution& resolution,ImGui_ImplVulkanH_Window* wd,ImDrawData* draw_data)
// {

// }
void VulkanContextManager::init(GLFWwindow* window)
{
    // Create Window Surface
    VkSurfaceKHR surface;
    VkResult err = glfwCreateWindowSurface(mInstance, window, mAllocator, &surface);
    VulkanContextManager::check_vk_result(err);
    // Create Framebuffers
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    // ImGui_ImplVulkanH_Window* wd = &mMainWindowData;
    SetupVulkanWindow(surface, w, h);
    {
        mMainWindowData.ClearValue.color.float32[0] = 0;
        mMainWindowData.ClearValue.color.float32[1] = 0;
        mMainWindowData.ClearValue.color.float32[2] = 0;
        mMainWindowData.ClearValue.color.float32[3] = 1;
    }
}

void VulkanContextManager::cleanup(GLFWwindow* window)
{
    auto err = vkDeviceWaitIdle(mDevice);
    check_vk_result(err);

    vkDestroyDescriptorPool(mDevice, mDescriptorPool, mAllocator);
    vkDestroyDevice(mDevice, mAllocator);
    vkDestroyInstance(mInstance, mAllocator);
    // clean window
    ImGui_ImplVulkanH_DestroyWindow(mInstance, mDevice, &mMainWindowData, mAllocator);
}

void VulkanRender::loadModel(std::unique_ptr<Model> model)
{
    mModel = std::move(model);
}

void VulkanContextManager::check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

RENDER_TYPE VulkanRender::getType() const
{
    return mType;
}

void VulkanRender::cleanup()
{
    // TODO
}

void VulkanContextManager::createVertexBuffer(const std::shared_ptr<Model>& mModel)
{
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VkDeviceSize bufferSize = sizeof(mModel->mVertice) * mModel->mVertice.size();
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    //*****填充顶点缓冲区*****
    void* data;
    //将缓冲区内存映射(mapping the buffer memory)到CPU可访问的内存中完成
    vkMapMemory(mDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    //将顶点数据拷贝到映射内存中
    memcpy(data, mModel->mVertice.data(), (size_t)bufferSize);
    //取消映射
    vkUnmapMemory(mDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(mDevice, stagingBuffer, nullptr);
    vkFreeMemory(mDevice, stagingBufferMemory, nullptr);

}
// 创建索引缓冲区
void VulkanContextManager::createIndexBuffer(const std::shared_ptr<Model>& mModel) {
    VkDeviceSize bufferSize = sizeof(mModel->mFaceIndex[0]) * mModel->mFaceIndex.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(mDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, mModel->mFaceIndex.data(), (size_t)bufferSize);
    vkUnmapMemory(mDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(mDevice, stagingBuffer, nullptr);
    vkFreeMemory(mDevice, stagingBufferMemory, nullptr);
}

void VulkanContextManager::createUniformBuffer()
{
    VkDeviceSize bufferSize = sizeof(glm::mat4)*3;
    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer,
                 uniformBufferMemory);
}

uint32_t VulkanContextManager::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }


    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanContextManager::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 3;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(mDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
}

//创建缓冲区
void VulkanContextManager::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                  VkDeviceMemory& bufferMemory)
{
    //创建缓冲区
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(mDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    //内存需求
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(mDevice, buffer, &memRequirements);

    //内存分配
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(mDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }
    //将内存关联到缓冲区
    vkBindBufferMemory(mDevice, buffer, bufferMemory, 0);
}

//用于从一个缓冲区拷贝数据到另一个缓冲区
void VulkanContextManager::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(mDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(mDevice, commandPool, 1, &commandBuffer);
}

bool VulkanContextManager::IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties,
                                                const char* extension)
{
    for (const VkExtensionProperties& p : properties)
        if (strcmp(p.extensionName, extension) == 0)
            return true;
    return false;
}

void VulkanContextManager::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    VulkanContextManager* self = (VulkanContextManager*)glfwGetWindowUserPointer(window);
    ImGui_ImplVulkan_SetMinImageCount(self->mMinImageCount);
    ImGui_ImplVulkanH_CreateOrResizeWindow(self->mInstance,
                                           self->mPhysicalDevice, self->mDevice, &self->mMainWindowData,
                                           self->mQueueFamily,
                                           self->mAllocator, width, height, self->mMinImageCount);
    self->mMainWindowData.FrameIndex = 0;
    // TODO:
}

void VulkanContextManager::setCallback(GLFWwindow* window)
{
    glfwSetWindowSizeCallback(window, windowSizeCallback);
}

void VulkanContextManager::resizeWindowSize(GLFWwindow* window)
{
    if (mSwapChainRebuild)
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (width > 0 && height > 0)
        {
            ImGui_ImplVulkan_SetMinImageCount(mMinImageCount);
            ImGui_ImplVulkanH_CreateOrResizeWindow(mInstance, mPhysicalDevice, mDevice, &mMainWindowData, mQueueFamily,
                                                   mAllocator, width, height, mMinImageCount);
            mMainWindowData.FrameIndex = 0;
            mSwapChainRebuild = false;
        }
    }
}


void VulkanContextManager::render(ImDrawData* draw_data)
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    mMainWindowData.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
    mMainWindowData.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
    mMainWindowData.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
    mMainWindowData.ClearValue.color.float32[3] = clear_color.w;
    VkResult err;

    VkSemaphore image_acquired_semaphore = mMainWindowData.FrameSemaphores[mMainWindowData.SemaphoreIndex].
        ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = mMainWindowData.FrameSemaphores[mMainWindowData.SemaphoreIndex].
        RenderCompleteSemaphore;
    err = vkAcquireNextImageKHR(mDevice, mMainWindowData.Swapchain, UINT64_MAX, image_acquired_semaphore,
                                VK_NULL_HANDLE, &mMainWindowData.FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        mSwapChainRebuild = true;
        return;
    }
    VulkanContextManager::check_vk_result(err);

    ImGui_ImplVulkanH_Frame* fd = &mMainWindowData.Frames[mMainWindowData.FrameIndex];
    {
        err = vkWaitForFences(mDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX);
        // wait indefinitely instead of periodically checking
        VulkanContextManager::check_vk_result(err);

        err = vkResetFences(mDevice, 1, &fd->Fence);
        VulkanContextManager::check_vk_result(err);
    }
    {
        err = vkResetCommandPool(mDevice, fd->CommandPool, 0);
        VulkanContextManager::check_vk_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        VulkanContextManager::check_vk_result(err);
    }
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = mMainWindowData.RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = mMainWindowData.Width;
        info.renderArea.extent.height = mMainWindowData.Height;
        info.clearValueCount = 1;
        info.pClearValues = &mMainWindowData.ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);


    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        VulkanContextManager::check_vk_result(err);
        err = vkQueueSubmit(mQueue, 1, &info, fd->Fence);
        VulkanContextManager::check_vk_result(err);
    }

    {
        // VulkanRender::FramePresent(ImGui_ImplVulkanH_Window* wd)
        if (mSwapChainRebuild)
            return;
        VkSemaphore render_complete_semaphore = mMainWindowData.FrameSemaphores[mMainWindowData.SemaphoreIndex].
            RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &mMainWindowData.Swapchain;
        info.pImageIndices = &mMainWindowData.FrameIndex;
        err = vkQueuePresentKHR(mQueue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            mSwapChainRebuild = true;
            return;
        }
        VulkanContextManager::check_vk_result(err);
        mMainWindowData.SemaphoreIndex = (mMainWindowData.SemaphoreIndex + 1) % mMainWindowData.SemaphoreCount;
        // Now we can use the next set of semaphores
    }
}


VkPhysicalDevice VulkanContextManager::SetupVulkan_SelectPhysicalDevice()
{
    uint32_t gpu_count;
    VkResult err = vkEnumeratePhysicalDevices(mInstance, &gpu_count, nullptr);
    check_vk_result(err);
    IM_ASSERT(gpu_count > 0);

    ImVector<VkPhysicalDevice> gpus;
    gpus.resize(gpu_count);
    err = vkEnumeratePhysicalDevices(mInstance, &gpu_count, gpus.Data);
    check_vk_result(err);

    // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
    // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
    // dedicated GPUs) is out of scope of this sample.
    for (VkPhysicalDevice& device : gpus)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            return device;
    }

    // Use first GPU (Integrated) is a Discrete one is not available.
    if (gpu_count > 0)
        return gpus[0];
    return VK_NULL_HANDLE;
}

void VulkanContextManager::SetupVulkan(ImVector<const char*> instance_extensions)
{
    VkResult err;

    // Create Vulkan Instance
    {
        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        // Enumerate available extensions
        uint32_t properties_count;
        ImVector<VkExtensionProperties> properties;
        vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
        properties.resize(properties_count);
        err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
        check_vk_result(err);

        // Enable required extensions
        if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
            instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

        // Enabling validation layers

        // Create Vulkan Instance
        create_info.enabledExtensionCount = (uint32_t)instance_extensions.Size;
        create_info.ppEnabledExtensionNames = instance_extensions.Data;
        err = vkCreateInstance(&create_info, mAllocator, &mInstance);
        check_vk_result(err);
    }

    // Select Physical Device (GPU)
    mPhysicalDevice = SetupVulkan_SelectPhysicalDevice();

    // Select graphics queue family
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &count, nullptr);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &count, queues);
        for (uint32_t i = 0; i < count; i++)
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                mQueueFamily = i;
                break;
            }
        free(queues);
        IM_ASSERT(mQueueFamily != (uint32_t)-1);
    }

    // Create Logical Device (with 1 queue)
    {
        ImVector<const char*> device_extensions;
        device_extensions.push_back("VK_KHR_swapchain");

        // Enumerate physical device extension
        uint32_t properties_count;
        ImVector<VkExtensionProperties> properties;
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &properties_count, nullptr);
        properties.resize(properties_count);
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &properties_count, properties.Data);


        const float queue_priority[] = {1.0f};
        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = mQueueFamily;
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = queue_priority;
        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = (uint32_t)device_extensions.Size;
        create_info.ppEnabledExtensionNames = device_extensions.Data;
        err = vkCreateDevice(mPhysicalDevice, &create_info, mAllocator, &mDevice);
        check_vk_result(err);
        vkGetDeviceQueue(mDevice, mQueueFamily, 0, &mQueue);
    }

    // Create Descriptor Pool
    // The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
    // If you wish to load e.g. additional textures you may need to alter pools sizes.
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,2}
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1;
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        err = vkCreateDescriptorPool(mDevice, &pool_info, mAllocator, &mDescriptorPool);
        check_vk_result(err);
    }
}

void VulkanContextManager::SetupVulkanWindow(VkSurfaceKHR surface, int width, int height)
{
    mMainWindowData.Surface = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, mQueueFamily, mMainWindowData.Surface, &res);
    if (res != VK_TRUE)
    {
        fprintf(stderr, "Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = {
        VK_FORMAT_B8G8R8A8_UNORM,
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_FORMAT_B8G8R8_UNORM,
        VK_FORMAT_R8G8B8_UNORM
    };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    mMainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
        mPhysicalDevice,
        mMainWindowData.Surface,
        requestSurfaceImageFormat,
        (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat),
        requestSurfaceColorSpace
    );

    // Select Present Mode
#ifdef APP_USE_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
#endif
    mMainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(
        mPhysicalDevice, mMainWindowData.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    //printf("[vulkan] Selected PresentMode = %d\n", mMainWindowData.PresentMode);

    // Create SwapChain, RenderPass, Framebuffer, etc.
    IM_ASSERT(mMinImageCount >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(mInstance, mPhysicalDevice, mDevice,
                                           &mMainWindowData, mQueueFamily, mAllocator, width, height, mMinImageCount);
}
