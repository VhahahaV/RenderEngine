//
// Created by czq on 24-6-8.
//

#ifndef RENDERBASE_H
#define RENDERBASE_H
#include "CameraBase.h"
#include "ParseStruct.h"
#include "Model.h"
#include <imgui_impl_vulkan.h>
#include <GLFW/glfw3.h>


class RenderBase{
private:
    RENDER_TYPE mType{};

public:
    virtual ~RenderBase() = default;
    virtual void render(std::shared_ptr<CameraBase> camera,const Resolution &resolution, ImDrawData* draw_data) = 0;
    virtual void init() = 0;
    virtual void cleanup() = 0;

    virtual void loadModel(std::unique_ptr<Model> model) = 0;
    [[nodiscard]] virtual RENDER_TYPE getType() const = 0;
}; // class RenderBase

class ContextManager
{
public:
    virtual GLFWwindow* makeContext() = 0;
    virtual void cleanup(GLFWwindow* window) = 0;
    virtual void setupPlatform(GLFWwindow* window) = 0;
    virtual void init(GLFWwindow* window) = 0;
    virtual void render(ImDrawData* draw_data) = 0;
    virtual void setCallback(GLFWwindow* window) = 0;
};

#endif //RENDERBASE_H


