//
// Created by czq on 24-6-8.
//

#ifndef RENDERBASE_H
#define RENDERBASE_H
#include "CameraBase.h"
#include "Model.h"


class RenderBase{
public:
    virtual void initRender() = 0;
    virtual void render(std::shared_ptr<CameraBase> camera,const Resolution &resolution) = 0;
    virtual void cleanup() = 0;
    virtual void loadModel(std::unique_ptr<Model> model) = 0;
}; // class RenderBase


#endif //RENDERBASE_H


