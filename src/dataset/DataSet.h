//
// Created by czq on 24-6-6.
//

#ifndef DATASET_H
#define DATASET_H


#include "DatasetBase.h"
#include <string>
#include "CameraUtils.h"
#include "Model.h"
#include "ParseStruct.h"
class DataSet : DatasetBase{

protected:

    std::string mJsonPath;
    Resolution mResolution;
    CameraInfo mCameraInfo;
    std::vector<ObjectInfo> mObjectInfos = {};

public:
    ~DataSet() = default;
    explicit DataSet(const std::string &filename);
    std::vector<std::unique_ptr<Model>> loadMesh() override;
    void getJsonPath() const override;
    void getMeshPath() const override;
    void debug();

};



#endif //DATASET_H
