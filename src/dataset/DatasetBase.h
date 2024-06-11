//
// Created by czq on 24-6-5.
//

#ifndef DATASETBASE_H
#define DATASETBASE_H
#include "Model.h"
#include "CameraBase.h"

class DatasetBase{
    virtual void getMeshPath() const= 0;
    virtual void getJsonPath() const= 0;
    virtual std::vector<std::unique_ptr<Model>> loadModels() = 0;
    virtual std::shared_ptr<CameraBase> loadCamera() = 0;

};
#endif //DATASETBASE_H
