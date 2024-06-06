//
// Created by czq on 24-6-4.
//
#include <argparse/argparse.hpp>
#include "Mesh.h"
int main(int argc, char *argv[]) {

    // std::cout << (input * input) << std::endl;
    ::loadByObj("resource/Wooden chair.obj","",false);
    ::loadByPly("resource/Wooden chair.ply",false,false);
    return 0;
}
