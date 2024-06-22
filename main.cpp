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
    std::shared_ptr<ContextManager> context = nullptr;
    std::shared_ptr<RenderBase> render = nullptr;
    context = std::make_shared<OpenglContextManager>();
    gui.loadContext(context);
    gui.initWindow({1280,720});
    render = std::make_shared<OpenglRender>();
    render->loadModel(std::move(testModel));

    // render.
    gui.loadRender(render);
    gui.loadCamera(camera);


    gui.init();
    // gui.setBackend();


    gui.mainLoop();
    gui.destroyWindow();
    return 0;
}
