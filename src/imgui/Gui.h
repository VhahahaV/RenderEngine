//
// Created by czq on 24-6-7.
//

#ifndef GUI_H
#define GUI_H
#include <cstdio>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "ParseStruct.h"
#include "CameraBase.h"

class Gui {
    GLFWwindow* mWindow = nullptr;
    inline static auto mGlslVersion = "#version 130";
    glm::vec4 mClearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::shared_ptr<CameraBase> mCamera = std::make_shared<CameraBase>();


public:
    // Decide GL+GLSL versions

    static void glfwErrorCallback(int error, const char* description);
    void loadCamera(std::shared_ptr<CameraBase> camera);
    void initWindow(Resolution resolution);
    void initImGui();
    void mainLoop();
    void showComponent();
    void getMouseWheel();
    void getMouseDrag();
    void getKeyBoard();
    void getInput();



};



#endif //GUI_H
