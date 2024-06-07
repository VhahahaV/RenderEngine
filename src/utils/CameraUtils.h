//
// Created by czq on 24-6-7.
//

#ifndef CAMERAUTILS_H
#define CAMERAUTILS_H
#include <glm/glm.hpp>
#include <array>
#include <vector>

enum class CAMERA_TYPE
{
    Perspective,
    Orthographic
};

glm::mat4 getModelMatrix(const std::array<float,3> &scale,const std::array<float,3> &position,const std::vector<float> &rotation);



#endif //CAMERAUTILS_H
