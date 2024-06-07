//
// Created by czq on 24-6-4.
//
#include <argparse/argparse.hpp>
#include "Model.h"
#include "DataSet.h"
#include "Gui.h"

int main(int argc, char *argv[]) {

    // std::cout << (input * input) << std::endl;
    // ::loadByObj("resource/Wooden chair.obj","",false);
    // ::loadByPly("resource/Wooden chair.ply",false,false);
    DataSet data_set("resource/test.json");
    Gui gui;
    gui.initWindow({1280,720});
    gui.initImGui();
    gui.mainLoop();

    return 0;
}
