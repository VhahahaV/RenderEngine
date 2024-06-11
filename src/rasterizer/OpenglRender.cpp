//
// Created by czq on 24-6-8.
//
#include "OpenglRender.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "debugFuncs.hpp"

const char *VERTEX_SHADER_CODE = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aLocalPos;


uniform mat4 view;
uniform mat4 project;
uniform mat4 model;
out vec4 vertexColor; // 为片段着色器指定一个颜色输出
void main(){
    //gl_Position = view * model * vec4(aPos, 1.0);
    gl_Position = project * view * model * vec4(aPos, 1.0);
    vertexColor = vec4(1.0, 1.0, 1.0, 1.0); // 把输出变量设置为暗红色
})glsl";

const char *FRAGMENT_SHADER_CODE = R"glsl(
#version 330 core
in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

layout (location = 0) out vec4 color;

void main(){
    color = vertexColor;
}
)glsl";

OpenglRender::OpenglRender()
{
    mVAO = mVBO = mEBO = mProgram = GL_INVALID_VALUE;
    mModel = nullptr;
    makeProgram();
}

void OpenglRender::initRender()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(mVAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    auto &vertices = mModel->mVertice;
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (float*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    const auto& indexes = mModel->mFaceIndex;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indexes.size(), indexes.data(), GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

}

void OpenglRender::render(std::shared_ptr<CameraBase> camera,const Resolution &resolution)
{
    // Render
    // Clear the colorbuffer


    // Draw our first triangle
    glUseProgram(mProgram);
    {
        // set uiniform
        auto viewLoc = glGetUniformLocation(mProgram, "view");
        auto projectLoc = glGetUniformLocation(mProgram, "project");
        auto modelLoc = glGetUniformLocation(mProgram, "model");
        auto &modelMatrix = mModel->mModelInfo.mModelMatrix;
        auto viewMatrix = camera->getViewMatrix();
        auto projectMatrix = camera->getProjMatrix(resolution);
        if constexpr (true)
        {
            if(viewLoc == GL_INVALID_VALUE)
            {
                throw std::runtime_error("TODO");
            }
            // TODO
        }
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectLoc, 1, GL_FALSE, &projectMatrix[0][0]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    glBindVertexArray(mVAO);
    if constexpr (1)
    {
        outputObj(
            multiplyMatrix(
            multiplyMatrix(
            multiplyMatrix(mModel->mVertice, mModel->mModelInfo.mModelMatrix),
            camera->getViewMatrix()
            ), camera->getProjMatrix(resolution)
            ),"MVP.obj");
        outputObj(
            multiplyMatrix(
            multiplyMatrix(mModel->mVertice, mModel->mModelInfo.mModelMatrix),
            camera->getViewMatrix()
            ), "MV.obj");
        outputObj(
            multiplyMatrix(
            multiplyMatrix(mModel->mVertice, mModel->mModelInfo.mModelMatrix),
            camera->getProjMatrix(resolution)
            ), "MP.obj");
        outputObj(
            multiplyMatrix(mModel->mVertice, mModel->mModelInfo.mModelMatrix)
            , "M.obj");
        outputObj(
            multiplyMatrix(mModel->mVertice, camera->getProjMatrix(resolution))
            , "P.obj");
        outputObj(
            multiplyMatrix(mModel->mVertice,
            camera->getViewMatrix()
            ), "V.obj");
        outputObj(
            mModel->mVertice
            , "None.obj");
    }
    glDrawElements(GL_TRIANGLES, GLsizei(mModel->mFaceIndex.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glFinish();
}

void OpenglRender::cleanup()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    // glfwTerminate();
}

void OpenglRender::makeProgram()
{
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VERTEX_SHADER_CODE, nullptr);
    glCompileShader(vertexShader);
    // Check for compile time errors
    checkError(vertexShader);
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_CODE, nullptr);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    checkError(fragmentShader);
    // Link shaders
    mProgram= glCreateProgram();
    glAttachShader(mProgram, vertexShader);
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void OpenglRender::loadModel(std::unique_ptr<Model> model)
{
    mModel = std::move(model);
}

void OpenglRender::checkError(const GLuint& shader)
{
    GLint success;
    static GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    }
}
