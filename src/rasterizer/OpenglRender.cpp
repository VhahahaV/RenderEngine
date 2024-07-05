//
// Created by czq on 24-6-8.
//
#include "OpenglRender.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "debugFuncs.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>

#include "FileUtils.h"


void OpenglContextManager::windowSizeCallback(GLFWwindow*, int width, int height)
{
}

void OpenglContextManager::cleanup(GLFWwindow* window)
{
    //TODO
}

OpenglRender::OpenglRender()
{
    mProgram = GL_INVALID_VALUE;
    // mVAOs = mTexCoords = mEBOs = nullptr;
    makeProgram();
}

void OpenglContextManager::setupPlatform(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
}

GLFWwindow* OpenglContextManager::makeContext()
{
    if (!glfwInit())
        throw std::runtime_error("glfw init error");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        throw std::runtime_error("window == nullptr");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowUserPointer(window, this);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error{"GLAD could not be loaded."};
    }
    return window;
}

void OpenglContextManager::render(ImDrawData* draw_data)
{
    // TODO
    ImGui_ImplOpenGL3_RenderDrawData(draw_data);
}

void OpenglContextManager::resizeWindowSize(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void OpenglContextManager::setCallback(GLFWwindow* window)
{
}

void OpenglContextManager::init(GLFWwindow* window)
{
    // TODO
}


void OpenglRender::init()
{
    for (int modelId = 0 ; modelId < mModels.size() ; modelId++)
    {
        const auto& model = mModels[modelId];
        auto& rasterizer = mRasterizers[modelId];
        auto shapeCount = model->getShapeCount();
        auto materialCount = model->getMaterialCount();
        glGenVertexArrays(shapeCount, rasterizer.mVAOs.data());
        glGenBuffers(shapeCount, rasterizer.mTexCoords.data());
        glGenBuffers(shapeCount, rasterizer.mEBOs.data());
        glGenBuffers(shapeCount, rasterizer.mVBOs.data());

        //Textures
        glGenTextures(materialCount, rasterizer.mTexs.data());
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

        for (int i = 0; i < shapeCount; i++)
        {
            glBindVertexArray(rasterizer.mVAOs[i]);
            // VBO
            glBindBuffer(GL_ARRAY_BUFFER, rasterizer.mVBOs[i]);
            auto& vertices = model->mShapes[i].mVertices;
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (float*)nullptr);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // texture coord attribute
            glBindBuffer(GL_ARRAY_BUFFER, rasterizer.mTexCoords[i]);
            auto& texcoords = model->mShapes[i].mTexCoords;
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (float*)nullptr);
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // EBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rasterizer.mEBOs[i]);
            const auto& indexes = model->mShapes[i].mIndices;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

            auto materialIndex = model->mShapes[i].mMaterialIndex;
            auto &material = model->mMaterials[materialIndex];
            //Textures
            glBindTexture(GL_TEXTURE_2D, rasterizer.mTexs[materialIndex]);
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // load image, create texture and generate mipmaps
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            if(!existPath(material.mPath))
            {
                std::cout << material.mPath << " not exist" << std::endl;
                continue;
            }
            unsigned char* data = stbi_load(material.mPath.c_str(), &width, &height,
                                            &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA,
                             GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);
            glBindVertexArray(0);
        }

    }

}

// }
// glBindVertexArray(mVAO);
//
// // VBO
// glBindBuffer(GL_ARRAY_BUFFER, mVBO);
// auto &vertices = mModel->mVertice;
// glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (float*)nullptr);
// glEnableVertexAttribArray(0);
// glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//
// // texture coord attribute
// glBindBuffer(GL_ARRAY_BUFFER, mTexCoords);
// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mModel->mTexCoords.size(), mModel->mTexCoords.data(), GL_STATIC_DRAW);
// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (float*)nullptr);
// glEnableVertexAttribArray(1);
// glBindBuffer(GL_ARRAY_BUFFER, 0);
//
// // EBO
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
// const auto& indexes = mModel->mFaceIndex;
// glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indexes.size(), indexes.data(), GL_STATIC_DRAW);
//
// //Textures
// for(auto &Tex : mTexs)
// {
//     glBindTexture(GL_TEXTURE_2D, Tex);
//     // set the texture wrapping parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     // set texture filtering parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     // load image, create texture and generate mipmaps
//     int width, height, nrChannels;
//     stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//     // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
//     // unsigned char *data = stbi_load(mModel->mMaterials[0].path.c_str(), &width, &height, &nrChannels, 0);
//     unsigned char *data = stbi_load("resource/obj/SJTU_east_gate_MC/East_Gate_Voxel-RGB.png", &width, &height, &nrChannels, 0);
//
//     if (data)
//     {
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 3? GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//     }
//     else
//     {
//         std::cout << "Failed to load texture" << std::endl;
//     }
//     stbi_image_free(data);
// }
// glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


void OpenglRender::render(std::shared_ptr<CameraBase> camera, const Resolution& resolution)
{
    // Render
    // Clear the colorbuffer
    glEnable(GL_DEPTH_TEST);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mProgram);

    // 设置视图和投影矩阵
    auto viewLoc = glGetUniformLocation(mProgram, "view");
    auto projectLoc = glGetUniformLocation(mProgram, "project");
    auto viewMatrix = camera->getViewMatrix();
    auto projectMatrix = camera->getProjMatrix(resolution);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectLoc, 1, GL_FALSE, &projectMatrix[0][0]);
    for (int modelId = 0 ; modelId < mModels.size() ; modelId++)
    {
        const auto& model = mModels[modelId];
        const auto& rasterizer = mRasterizers[modelId];
        auto shapeCount = model->getShapeCount();
        auto materialCount = model->getMaterialCount();


        // 遍历每个Shape进行绘制
        for (int i = 0; i < shapeCount; i++)
        {
            // 设置模型矩阵
            auto modelLoc = glGetUniformLocation(mProgram, "model");
            auto& modelMatrix = model->mModelInfo.mModelMatrix;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);

            glBindVertexArray(rasterizer.mVAOs[i]);

            // 绑定纹理
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, rasterizer.mTexs[model->mShapes[i].mMaterialIndex]);
            // auto texLoc = glGetUniformLocation(mProgram, "ourTexture");
            // glUniform1i(texLoc, 0);
            // glUniform1i(glGetUniformLocation(mProgram, "ourTexture"), 0);

            // 使用每个面的顶点数进行绘制
            const auto& shape = model->mShapes[i];
            size_t index_offset = 0;
            for (size_t j = 0; j < shape.mNumFaceVertices.size(); ++j)
            {
                int numVertices = shape.mNumFaceVertices[j];
                glDrawElements(GL_TRIANGLE_FAN, numVertices, GL_UNSIGNED_INT, (void*)(index_offset * sizeof(unsigned int)));
                index_offset += numVertices;
            }
            glBindVertexArray(0);
        }
    }

    glFinish();
}

void OpenglRender::cleanup()
{
    // Properly de-allocate all resources once they've outlived their purpose
    // glDeleteVertexArrays(1, &mVAO);
    // glDeleteBuffers(1, &mVBO);
    // glDeleteBuffers(1, &mEBO);
    // glDeleteBuffers(1, &mTexCoords);
    // glDeleteTextures(1, &mTex);
    for (int modelId = 0 ; modelId < mModels.size() ; modelId++)
    {
        auto &model = mModels[modelId];
        auto &rasterizer = mRasterizers[modelId];
        auto shapeCount = model->getShapeCount();
        for (int i = 0; i < shapeCount; i++)
        {
            glDeleteVertexArrays(1, &rasterizer.mVAOs[i]);
            glDeleteBuffers(1, &rasterizer.mVBOs[i]);
            glDeleteBuffers(1, &rasterizer.mEBOs[i]);
            glDeleteBuffers(1, &rasterizer.mTexCoords[i]);
        }
        auto materialCount = model->getMaterialCount();
        for (int i = 0; i < materialCount; i++)
            glDeleteTextures(1, &rasterizer.mTexs[i]);
    }


    glDeleteProgram(mProgram);
}

void OpenglRender::makeProgram()
{
    // Build and compile our shader program
    // Vertex shader
    // 1. 从文件路径中获取顶点/片段着色器
    const char* vertexPath = "src/shaders/OpenGL/OpenGLVertexShader.vert";
    const char* fragmentPath = "src/shaders/OpenGL/OpenGLFragmentShader.frag";
    // const char *vertexPath = "src/shaders/OpenGL/OpenGLUvDebbugShader.vert";
    // const char *fragmentPath = "src/shaders/OpenGL/OpenGLUvDebbugShader.frag";
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        // 转换数据流到string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);
    // Check for compile time errors
    checkError(vertexShader);
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    checkError(fragmentShader);
    // Link shaders
    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertexShader);
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenglRender::loadModel(std::shared_ptr<Model> model)
{
    Rasterizer rasterizer;
    rasterizer.mEBOs.resize(model->getShapeCount(),GL_INVALID_VALUE);
    rasterizer.mVAOs.resize(model->getShapeCount(),GL_INVALID_VALUE);
    rasterizer.mTexCoords.resize(model->getShapeCount(),GL_INVALID_VALUE);
    rasterizer.mVBOs.resize(model->getShapeCount(),GL_INVALID_VALUE);
    rasterizer.mTexs.resize(model->getMaterialCount(),GL_INVALID_VALUE);
    mRasterizers.emplace_back(rasterizer);
    mModels.emplace_back(std::move(model));

}

void OpenglRender::loadManager(std::shared_ptr<ContextManager> manager)
{
}

RENDER_TYPE OpenglRender::getType() const
{
    return mType;
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
