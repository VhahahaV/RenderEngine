//
// Created by czq on 24-6-5.
//

#ifndef DATASETBASE_H
#define DATASETBASE_H
#include "Model.h"

class DatasetBase{
    virtual void getMeshPath() const= 0;
    virtual void getJsonPath() const= 0;
    virtual std::vector<std::unique_ptr<Model>> loadMesh() = 0;

};
#endif //DATASETBASE_H
