//
// Created by czq on 24-6-7.
//

#ifndef CAMERAUTILS_H
#define CAMERAUTILS_H
#include <glm/glm.hpp>
#include <array>
#include <vector>


glm::mat4 getModelMatrix(const std::array<float,3> &scale,const std::array<float,3> &position,const std::vector<float> &rotation);

// 相机内参矩阵K和fov的相互转换参考：https://blog.csdn.net/Vpn_zc/article/details/125976495


#endif //CAMERAUTILS_H
