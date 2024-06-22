//
// Created by czq on 24-6-8.
//

#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H
#include "Model.h"
#include <glad/glad.h>

#include "RenderBase.h"
class OpenglContextManager: public ContextManager
{
public:
    GLFWwindow* makeContext() override;
    void cleanup(GLFWwindow* window) override;
    void setupPlatform(GLFWwindow* window) override;
    void init(GLFWwindow* window) override;
    void render(ImDrawData* draw_data) override;
    void resizeWindowSize(GLFWwindow* window) override;
    void setCallback(GLFWwindow* window) override;

public:// callbacks
    static void windowSizeCallback(GLFWwindow*, int, int);

};

class OpenglRender : public RenderBase{
private:
    RENDER_TYPE mType = RENDER_TYPE::OpenGL;
    GLuint mVBO, mEBO,mVAO,mProgram;
    std::unique_ptr<Model> mModel;
public:
    OpenglRender();
    void init() override;
    void render(std::shared_ptr<CameraBase> camera,const Resolution &resolution) override;
    void cleanup() override;
    void makeProgram();
    void loadModel(std::unique_ptr<Model> model) override;
    [[nodiscard]] RENDER_TYPE getType() const override;
    static void checkError(const GLuint &shader);
};



#endif //OPENGLRENDER_H
