//
// Created by czq on 24-6-6.
//

#include "DataSet.h"

#include <filesystem>
#include <fstream>
#include <rapidjson/document.h>
#include <iostream>

#include "../camera/CameraBase.h"
#include "../camera/OrthographicCamera.h"
#include "../camera/PerspectiveCamera.h"

DataSet::DataSet(const std::string &filename)
{
    std::fstream f(filename);
    if (!f.good()) {
        throw std::runtime_error("open file '" + filename + "' error");
    }
    std::ostringstream o;
    o << f.rdbuf();
    std::string rawJson(std::move(o.str()));
    rapidjson::Document doc;
    doc.Parse(rawJson.c_str());
    auto throwParseError = [&]()
    {
        throw std::runtime_error("in the file '" + filename + ", can't parse an Object");
    };
    if(!doc.IsObject())
        throwParseError();
    const rapidjson::Value& resolutoins = doc["resolution"];
    if(resolutoins.IsArray())
    {
        mResolution.InitResolution(resolutoins[0].GetFloat(),resolutoins[1].GetFloat());
    }else throwParseError();
    const rapidjson::Value& camera = doc["camera"];
    if(camera.IsObject())
    {
        std::string typeStr = camera["type"].GetString();
        CAMERA_TYPE type;
        if(typeStr == "perspective") type = CAMERA_TYPE::Perspective;
        else if(typeStr == "orthographic") type = CAMERA_TYPE::Orthographic;
        else throwParseError();
        mCameraInfo.InitCameraInfo(type,camera["fx"].GetFloat(),camera["fy"].GetFloat(),camera["cx"].GetFloat(),camera["cy"].GetFloat());
    }else throwParseError();
    const rapidjson::Value& objects = doc["objects"];
    if(objects.IsArray())
    {
        for(auto &obj : objects.GetArray())
        {
            const rapidjson::Value& scale = obj["scale"];
            std::array<float,3> scaleArr{scale[0].GetFloat(),scale[1].GetFloat(),scale[2].GetFloat()};
            const rapidjson::Value& position = obj["position"];
            std::array<float,3> positionArr{position[0].GetFloat(),position[1].GetFloat(),position[2].GetFloat()};
            const rapidjson::Value& rotation = obj["rotation"];
            std::vector<float> rotationVec;
            std::string rotationType;
            if(!rotation.IsObject())
                throwParseError();
            else
                for (auto &val : rotation["data"].GetArray())
                    rotationVec.emplace_back(val.GetFloat());
            std::string typeStr = obj["type"].GetString();
            MODEL_TYPE type;
            if(typeStr == ".obj") type = MODEL_TYPE::Obj;
            else if(typeStr == ".ply") type = MODEL_TYPE::Ply;
            else throwParseError();
            ModelInfo tmp(
                obj["name"].GetString(),
                type,
                obj["file"].GetString(),
                getModelMatrix(scaleArr,positionArr,rotationVec)
            );
            mObjectInfos.emplace_back(std::move(tmp));
        }
    }else throwParseError();

    debug();
}

std::vector<std::unique_ptr<Model>> DataSet::loadModels()
{
    std::vector<std::unique_ptr<Model>> models;
    for (auto &model : mObjectInfos)
    {
        if(model.mType == MODEL_TYPE::Obj)
        {
            // models.emplace((::loadByObj(model,false));
            models.emplace_back(std::move(::loadByObj(model,false)));
        }
        else if(model.mType == MODEL_TYPE::Ply)
        {
            // loadByPly(model);
            models.emplace_back(std::move(::loadByPly(model,false,false)));
        }
    }
    return models;
}

std::shared_ptr<CameraBase> DataSet::loadCamera()
{
    std::shared_ptr<CameraBase> camera = nullptr;
    if(mCameraInfo.mType == CAMERA_TYPE::Perspective)
    {
        camera = std::make_shared<PerspectiveCamera>();
    }
    else if(mCameraInfo.mType == CAMERA_TYPE::Orthographic)
    {
        camera = std::make_shared<OrthographicCamera>();
    }
    return camera;
}

void DataSet::getJsonPath() const
{
}


void DataSet::getMeshPath() const
{
}

void DataSet::debug()
{
std::cout << "Resolution: " << mResolution.w << " " << mResolution.h << std::endl;
    std::cout << "Camera: " <<  " " << mCameraInfo.mFx << " " << mCameraInfo.mFy << " " << mCameraInfo.mCx << " " << mCameraInfo.mCy << std::endl;
    for(auto &obj : mObjectInfos)
    {
        std::cout << "Object: " << obj.mName << " " << " " << obj.mFilename << std::endl;
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                std::cout << obj.mModelMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}
