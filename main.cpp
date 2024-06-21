//
// Created by czq on 24-6-4.
//
#include <argparse/argparse.hpp>
#include "OpenglRender.h"
#include "VulkanRender.h"
 #include "Model.h"
#include "DataSet.h"
#include "Gui.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

int main(int argc, char *argv[]) {

    // ::loadByObj("resource/Wooden chair.obj","",false);
    // ::loadByPly("resource/Wooden chair.ply",false,false);
    DataSet data_set("resource/test.json");
    auto models = data_set.loadModels();
    auto testModel = std::move(models.front());
    std::shared_ptr<CameraBase> camera = nullptr;
    camera = std::make_shared<PerspectiveCamera>();
    Gui gui;
    gui.initWindow({1280,720});
    std::shared_ptr<RenderBase> render = nullptr;
    render = std::make_shared<VulkanRender>();
    render->loadModel(std::move(testModel));

    // render.
    gui.loadRender(std::move(render));
    gui.loadCamera(camera);


    gui.init();
    // gui.setBackend();


    gui.mainLoop();
    gui.destroyWindow();
    return 0;
}
