//
// Created by czq on 24-6-7.
//

#include "CameraUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
glm::mat4 getModelMatrix(const std::array<float,3> &scale,const std::array<float,3> &position,const std::vector<float> &rotation)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model,glm::vec3(scale[0],scale[1],scale[2]));
    model = glm::translate(model,glm::vec3(position[0],position[1],position[2]));

    if(rotation.size() == 3)
    {
        // euler_xyz
        model = glm::rotate(model,glm::radians(rotation[0]),glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(rotation[1]),glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model,glm::radians(rotation[2]),glm::vec3(0.0f,0.0f,1.0f));
    }
    else if(rotation.size() == 4)
    {
        // quaternion
        glm::quat q = glm::quat(rotation[0],rotation[1],rotation[2],rotation[3]);
        model = model * glm::mat4_cast(q);
    }
    return model;
}
