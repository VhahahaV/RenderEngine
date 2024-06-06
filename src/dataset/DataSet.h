//
// Created by czq on 24-6-6.
//

#ifndef DATASET_H
#define DATASET_H


#include "DatasetBase.h"
#include <iostream>
#include <string>
class DataSet : DatasetBase{

protected:
    std::string mJsonPath;
    struct Resolution
    {
        uint32_t w;
        uint32_t h;
        Resolution(uint32_t x,uint32_t y):w(x),h(y){}
    } mResolution;

public:
    void

};



#endif //DATASET_H
