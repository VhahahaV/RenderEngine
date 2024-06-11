//
// Created by czq on 24-6-7.
//

#ifndef GUI_H
#define GUI_H
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <memory>
#include "ParseStruct.h"
#include "CameraBase.h"
#include "RenderBase.h"

class Gui {
    GLFWwindow* mWindow = nullptr;
    Resolution mResolution{};
    inline static auto mGlslVersion = "#version 130";
    glm::vec4 mClearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::shared_ptr<CameraBase> mCamera{};
    std::shared_ptr<RenderBase> mRender{};

public:
    // Decide GL+GLSL versions

    static void glfwErrorCallback(int error, const char* description);
    void loadCamera(std::shared_ptr<CameraBase> camera);
    void loadRender(std::shared_ptr<RenderBase> render);
    void initWindow(Resolution resolution);
    void initImGui();
    void mainLoop();
    void resetBackground();
    void destroyWindow();
    void showComponent();
    void getMouseWheel();
    void getMouseDrag();
    void getKeyBoard();
    void getInput();



};



#endif //GUI_H
