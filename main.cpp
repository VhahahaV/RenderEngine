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
    // 创建 ArgumentParser 对象
    argparse::ArgumentParser program("ToyRenderEngine");

    // 添加参数
    program.add_argument("-c", "--camera")
        .default_value(std::string("PerspectiveCamera"))
        .help("Camera type (PerspectiveCamera or OrthographicCamera)");

    program.add_argument("-j", "--json")
        .default_value(std::string("resource/json/test3.json"))
        .help("Path to the JSON file");

    program.add_argument("-r", "--renderer")
        .default_value(std::string("OpenglRender"))
        .help("Renderer type (OpenglRender or VulkanRender)");

    try {
        // 解析命令行参数
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    // 获取参数值
    std::string camera_type = program.get<std::string>("--camera");
    std::string json_path = program.get<std::string>("--json");
    std::string renderer_type = program.get<std::string>("--renderer");

    // 加载模型
    DataSet data_set(json_path);
    auto models = data_set.loadModels();
    // auto testModel = std::move(models.front());

    // 选择相机
    std::shared_ptr<CameraBase> camera = nullptr;
    if (camera_type == "OrthographicCamera") {
        camera = std::make_shared<OrthographicCamera>();
    } else {
        camera = std::make_shared<PerspectiveCamera>();
    }

    // 初始化GUI
    Gui gui;
    std::shared_ptr<ContextManager> context = nullptr;
    std::shared_ptr<RenderBase> render = nullptr;

    // 选择渲染器
    if (renderer_type == "VulkanRender") {
        context = std::make_shared<VulkanContextManager>();
        gui.loadContext(context);
        gui.initWindow({1280,720});
        render = std::make_shared<VulkanRender>();
    } else {
        context = std::make_shared<OpenglContextManager>();
        gui.loadContext(context);
        gui.initWindow({1280,720});
        render = std::make_shared<OpenglRender>();
    }
    for(auto &model : models)
    {
        render->loadModel(std::move(model));
    }
    gui.loadRender(render);
    gui.loadCamera(camera);

    gui.init();
    gui.mainLoop();
    gui.destroyWindow();
    return 0;
}
