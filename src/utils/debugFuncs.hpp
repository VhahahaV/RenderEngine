#pragma once

#include <fstream>
#include <iomanip>
#include <vector>
#include <glm/glm.hpp>

void outputObj(const std::vector<float>& vert, const std::string& fileName){
    if(vert.size() % 3 != 0)
    {
        throw std::runtime_error("vert.size() % 3 != 0");
    }
    std::ofstream f(fileName, std::ios::out);
    if(!f.good()){
        throw std::runtime_error("open file '"+fileName+"' error");
    }
    f << std::fixed << std::setprecision(7);
    for(size_t i=0; i<vert.size();)
    {
        f << "v " << vert[i++] << ' ' << vert[i++] << ' ' << vert[i++] << '\n';
    }
}

std::vector<float> multiplyMatrix(const std::vector<float>& vert, const glm::mat4& m)
{
    std::vector<float> result; result.reserve(vert.size());
    for(size_t i=0; i<vert.size();)
    {
        glm::vec4 v;
        v[0] = vert[i++];
        v[1] = vert[i++];
        v[2] = vert[i++];
        v[3] = 1;
        auto v_ = m * v;
        result.emplace_back(v_.x/v_[3]);
        result.emplace_back(v_.y/v_[3]);
        result.emplace_back(v_.z/v_[3]);
    }
    return result;
}