//
// Created by czq on 24-6-8.
//

#ifndef RENDERBASE_H
#define RENDERBASE_H
#include "CameraBase.h"
#include "ParseStruct.h"
#include "Model.h"
#include <imgui_impl_vulkan.h>



class RenderBase{
private:
    RENDER_TYPE mType{};
public:
    virtual ~RenderBase() = default;
    virtual void setupPlatform(ImGui_ImplVulkanH_Window* wd) = 0;
    virtual void initRender() = 0;
    virtual void render(std::shared_ptr<CameraBase> camera,const Resolution &resolution) = 0;
    virtual void cleanup(ImGui_ImplVulkanH_Window* wd) = 0;
    virtual void loadModel(std::unique_ptr<Model> model) = 0;
    [[nodiscard]] virtual RENDER_TYPE getType() const = 0;
}; // class RenderBase


#endif //RENDERBASE_H


