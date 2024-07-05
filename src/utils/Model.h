#ifndef MESH_H
#define MESH_H

#include <memory>
#include <vector>
#include <iostream>
#include <array>
#include <string>
#include "ParseStruct.h"

struct Material {
    std::string mName;
    std::string mPath;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;
    float shininess;
    std::string texture_diffuse;
    std::string texture_specular;
    std::string texture_normal;
};

struct Shape {
    std::string mName;
    uint32_t mMaterialIndex;
    std::vector<float> mVertices{}; // 每个shape顶点缓存
    std::vector<uint32_t> mIndices{}; // 索引
    std::vector<float> mNormals{}; // 每个shape法线法线缓存
    std::vector<float> mTexCoords{}; // 每个shape纹理坐标缓存
    std::vector<uint32_t> mNumFaceVertices{}; // 每个面顶点数量
    Shape(const std::string& name, uint32_t materialIndex)
        : mName(name), mMaterialIndex(materialIndex) {}
};

struct   Model {
    ModelInfo mModelInfo{};
    std::vector<Shape> mShapes{};
    std::vector<Material> mMaterials;

    [[nodiscard]] uint32_t getShapeCount() const;
    [[nodiscard]] uint32_t getMaterialCount() const;
    void debug() const {
        for (size_t i = 0 ; i < mShapes.size(); ++i) {
            const auto& shape = mShapes[i];
            std::cout << "Shape[" << i << "]:\n";
            std::cout << "  Name: " << shape.mName << '\n';
            std::cout << "  Material Index: " << shape.mMaterialIndex << '\n';
            std::cout << "  Vertices: Count = " << shape.mVertices.size() << '\n';
            std::cout << "  Indices: Count = " << shape.mIndices.size() << '\n';
            std::cout << "  Normals: Count = " << shape.mNormals.size() << '\n';
            std::cout << "  TexCoords: Count = " << shape.mTexCoords.size() << '\n';
            std::cout << "  NumFaceVertices: Count = " << shape.mNumFaceVertices.size() << '\n';
        }
        std::cout << "Materials: Count = " << mMaterials.size() << '\n';
        for (size_t i = 0; i < mMaterials.size(); ++i) {
            const auto& material = mMaterials[i];
            std::cout << "  Material[" << i << "]:\n";
            std::cout << "    Path: " << material.mPath << '\n';
            std::cout << "    Ambient: (" << material.ambient[0] << ", " << material.ambient[1] << ", material.ambient[2] << " << std::endl;
            std::cout << "    Diffuse: (" << material.diffuse[0] << ", " << material.diffuse[1] << ", material.diffuse[2] << " << std::endl;
            std::cout << "    Specular: (" << material.specular[0] << ", " << material.specular[1] << ", material.specular[2] << " << std::endl;
            std::cout << "    Shininess: " << material.shininess << '\n';
            std::cout << "    Texture Diffuse: " << material.texture_diffuse << '\n';
            std::cout << "    Texture Specular: " << material.texture_specular << '\n';
            std::cout << "    Texture Normal: " << material.texture_normal << '\n';
        }
        std::cout << "--------------------------------------------------------------------------\n";
    }
};

std::unique_ptr<Model> loadByObj(const ModelInfo &modelInfo, bool triangulate);
std::unique_ptr<Model> loadByPly(const ModelInfo &modelInfo, bool requireNormal, bool requireTexcoord);

#endif // MESH_H
