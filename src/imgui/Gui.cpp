//
// Created by czq on 24-6-7.
//

#include "Gui.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_vulkan.h>

#include <vector>


void Gui::glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Gui::loadCamera(std::shared_ptr<CameraBase> camera)
{
    mCamera = std::move(camera);
}

void Gui::loadRender(std::shared_ptr<RenderBase> render)
{
    mRender = std::move(render);
}

void Gui::initWindow(Resolution resolution)
{
    mResolution = resolution;
    // TODO: context init virtual function call
    glfwSetErrorCallback(glfwErrorCallback);
    // if (!glfwInit())
    //     return ;
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create window with graphics context
    mWindow = mContext->makeContext();
    // // load glad
    // if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    // {
    //     throw std::runtime_error{"GLAD could not be loaded."};
    // }



}

void Gui::init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    mContext->setupPlatform(mWindow);
    mRender->init();
}



void Gui::mainLoop()
{
    if(mWindow == nullptr || mResolution.h == 0 || mResolution.w == 0) {
        throw std::runtime_error("you need run GraphicsUI::initWindow before calling mainLoop");
    }

    while (!glfwWindowShouldClose(mWindow))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

        glfwPollEvents();


        if(mRender->getType() == RENDER_TYPE::OpenGL)
            ImGui_ImplOpenGL3_NewFrame();
        else if(mRender->getType() == RENDER_TYPE::Vulkan)
            ImGui_ImplVulkan_NewFrame();
        else
            throw std::runtime_error{"Render type not supported."};

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        getInput();
        showComponent();
        renderFrame();

    }

    cleanUp();
}

void Gui::renderFrame()
{
    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();

    if(mRender->getType() == RENDER_TYPE::OpenGL)
    {
        mRender->render(mCamera, mResolution, draw_data);
        ImGui_ImplOpenGL3_RenderDrawData(draw_data);
        glfwSwapBuffers(mWindow);
    }
    else if(mRender->getType() == RENDER_TYPE::Vulkan)
    {
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            mRender->render(mCamera, mResolution,draw_data);
        }
    }

}

void Gui::destroyWindow()
{
    if(mWindow != nullptr) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(mWindow);
        glfwTerminate();
        mWindow = nullptr;
    }
}

void Gui::showComponent()
{
    ImGui::Begin("RenderEngine");
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
        float local_fov = mCamera->getFov();
        if (ImGui::SliderFloat("fov", &local_fov, 69.0f, 71.0f)) {
            mCamera->setFov(local_fov);
        }

        ImGui::PopItemWidth();
        if (ImGui::TreeNode("Advanced camera settings")) {
            glm::vec3 local_screen_center = mCamera->getPosition();
            if (ImGui::SliderFloat3("camera center", &local_screen_center.x, 0.f, 2.0f))
                mCamera->setPosition(local_screen_center);

            static std::vector<char> buf(2048);
            glm::vec3 v = mCamera->getFront();
            glm::vec3 u = mCamera->getUp();
            int bufsize = snprintf(buf.data(), buf.size()-1,
                     "Up_dir = [%0.3f,%0.3f,%0.3f]\n"
                     "View_dir = [%0.3f,%0.3f,%0.3f]\n"
                     "Fov = %0.3f,\n"
                     "Scale = %0.3f \n"
                    ,u.x, u.y, u.z
                    ,v.x, v.y, v.z
                    , mCamera->getFov()
                    , mCamera->getScale()
            );
            ImGui::InputTextMultiline("Params", buf.data(), bufsize);
            ImGui::TreePop();

        }
    }
    ImGui::End();
}

void Gui::getMouseWheel()
{
    ////TODO
    ImGuiIO& io = ImGui::GetIO();
    ImGuiKey key = ImGuiKey_MouseWheelY;
    if (!ImGui::IsKeyDown(key))
        return;
    auto delta = io.MouseWheel;
    float scale_factor = pow(1.05f, -delta);
    mCamera->setScale(mCamera->getScale()*scale_factor);
}

void Gui::getMouseDrag()
{
    ////TODO
    ImGuiIO& io = ImGui::GetIO();
    if(io.MouseDown[0])
    {
        auto mouseDelta = glm::vec2(io.MouseDelta.x, io.MouseDelta.y);
        mCamera->updateView(mouseDelta, true);
    }


}

void Gui::getKeyBoard()
{
    ////TODO
    ImGuiIO& io = ImGui::GetIO();
    auto cameraMove = glm::vec3(0.f);
    if (ImGui::IsKeyDown(ImGuiKey_W))
        cameraMove.x += 1.0f;
    if (ImGui::IsKeyDown(ImGuiKey_A))
        cameraMove.y += 1.0f;
    if (ImGui::IsKeyDown(ImGuiKey_S))
        cameraMove.x -= 1.0f;
    if (ImGui::IsKeyDown(ImGuiKey_D))
        cameraMove.y -= 1.0f;
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        cameraMove.z -= 1.0f;
    if (ImGui::IsKeyDown(ImGuiKey_C))
        cameraMove.z += 1.0f;
    if(ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_Q)) {
        glfwSetWindowShouldClose(mWindow, true);
    }
    cameraMove *= io.DeltaTime;
    mCamera->processKeyboard(cameraMove);
}

void Gui::getInput()
{
    ////TODO
    ImGuiIO& io = ImGui::GetIO();
    if(io.WantCaptureKeyboard || io.WantCaptureMouse)
        return;
    getKeyBoard();
    getMouseWheel();
    getMouseDrag();

}

void Gui::cleanUp()
{
    // if (mRender && mRender->getType()==RENDER_TYPE::OpenGL)
    //     ImGui_ImplOpenGL3_Shutdown();
    // else if (mRender && mRender->getType()==RENDER_TYPE::Vulkan)
    //     ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    mRender->cleanup();
    mContext->cleanup(mWindow);

    glfwDestroyWindow(mWindow);
    glfwTerminate();
}
