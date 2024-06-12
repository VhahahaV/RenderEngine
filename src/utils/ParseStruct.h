//
// Created by czq on 24-6-7.
//

#ifndef PARSESTRUCT_H
#define PARSESTRUCT_H

#include <string>

#include "glm/glm.hpp"


enum class MODEL_TYPE
{
    Obj,
    Ply
};
enum class CAMERA_TYPE
{
    Perspective,
    Orthographic
};

enum class RENDER_TYPE
{
    OpenGL,
    Vulkan
};

struct Resolution
{
    float w=0;
    float h=0;
    void InitResolution(float x,float y)
    {
        w=x,h=y;
    }
};


struct CameraInfo
{
    CAMERA_TYPE mType{};
    float mFx=0,mFy=0,mCx=0,mCy=0;
    void InitCameraInfo(CAMERA_TYPE t,float x1,float y1 , float x2, float y2)
    {
        mType=t;mFx=x1;mFy=y1;mCx=x2;mCy=y2;
    }
};


struct ModelInfo
{
    std::string mName{};
    MODEL_TYPE mType{};
    std::string mFilename{};
    glm::mat4 mModelMatrix{};
    ModelInfo()=default;
    ModelInfo(std::string  n,MODEL_TYPE t,std::string f,const glm::mat4 &m):
        mName(std::move(n)),mType(t),mFilename(std::move(f)),mModelMatrix(m){}

    void InitObjectInfo(std::string  n,MODEL_TYPE t,std::string f,glm::mat4 m)
    {
        mName = std::move(n);
        mFilename = std::move(f),
        mType = t;
        mModelMatrix = m;
    }
};
#endif //PARSESTRUCT_H
