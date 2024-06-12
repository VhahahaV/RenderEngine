//
// Created by czq on 24-6-8.
//

#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H
#include "Model.h"
#include "RenderBase.h"
#include <glad/glad.h>

class OpenglRender : public RenderBase{
private:
    inline static auto mGlslVersion = "#version 130";
    RENDER_TYPE mType = RENDER_TYPE::OpenGL;
    GLuint mVBO, mEBO,mVAO,mProgram;
    std::unique_ptr<Model> mModel;
public:
    OpenglRender();
    void setupPlatform(ImGui_ImplVulkanH_Window* wd) override;
    void initRender() override;
    void render(std::shared_ptr<CameraBase> camera,const Resolution &resolution) override;
    void cleanup(ImGui_ImplVulkanH_Window* wd) override;
    void makeProgram();
    void loadModel(std::unique_ptr<Model> model) override;
    [[nodiscard]] RENDER_TYPE getType() const override;
    static void checkError(const GLuint &shader);
};



#endif //OPENGLRENDER_H
