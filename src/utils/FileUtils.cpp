//
// Created by czq on 24-7-1.
//

#include "FileUtils.h"
#include <string>
#include <stdexcept>

namespace fs = std::filesystem;

// 查找唯一的 .obj 文件
std::string findObj(const std::string& dir) {
    std::vector<std::string> objFiles;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".obj") {
            objFiles.push_back(entry.path().string());
        }
    }

    if (objFiles.size() != 1) {
        throw std::runtime_error("Expected exactly one .obj file, found " + std::to_string(objFiles.size()));
    }

    return objFiles.front();
}

// 查找唯一的 .mtl 文件
std::string findMtl(const std::string& dir) {
    std::vector<std::string> mtlFiles;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".mtl") {
            mtlFiles.push_back(entry.path().string());
        }
    }

    if (mtlFiles.size() != 1) {
        throw std::runtime_error("Expected exactly one .mtl file, found " + std::to_string(mtlFiles.size()));
    }

    return mtlFiles.front();
}

// 查找所有 .png 文件
std::vector<std::string> findPngs(const std::string& dir) {
    std::vector<std::string> pngFiles;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            pngFiles.push_back(entry.path().string());
        }
    }

    if (pngFiles.empty()) {
        throw std::runtime_error("No .png files found");
    }

    return pngFiles;
}

std::string getPath(const std::string& path)
{
    return nullptr;
}

bool existPath(const std::string& path)
{
    return fs::exists(path);
}
