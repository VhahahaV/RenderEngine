//
// Created by czq on 24-6-5.
//

#ifndef MESH_H
#define MESH_H



#include <memory>
#include <vector>
#include <iostream>

#include "ParseStruct.h"


struct  Model
{
    ModelInfo mModelInfo{};
    std::vector<float> mVertice{};
    std::vector<float> mNormal{};
    std::vector<float> mTexCoords{};
    std::vector<uint32_t> mFaceIndex{};
    void debug()
    {
        std::cout << "---------------------------MESH DEBUG-----------------------------------\n";
        std::cout << "get mVertice: nums:" << mVertice.size()<<'\n';
        std::cout << "get mNormal: nums:" << mNormal.size()<<'\n';
        std::cout << "get mTexCoords: nums:" << mTexCoords.size()<<'\n';
        std::cout << "get mFaceIndex: nums:" << mFaceIndex.size()<<'\n';
    }
};





std::unique_ptr<Model> loadByObj(const ModelInfo &modelInfo,bool triangulate);
std::unique_ptr<Model> loadByPly(const ModelInfo &modelInfo,bool requireNormal, bool requireTexcoord);


#endif //MESH_H
