//
// Created by czq on 24-6-5.
//

#ifndef DATASETBASE_H
#define DATASETBASE_H

class DatasetBase{
    virtual void getCamera() const = 0;
    virtual void getMeshPath() const= 0;
    virtual void getJsonPath() const= 0;
    virtual void loadJson() = 0;
    virtual void loadMesh() = 0;

};
#endif //DATASETBASE_H
