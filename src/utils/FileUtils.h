//
// Created by czq on 24-7-1.
//

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <filesystem>
#include <vector>

std::string findObj(const std::string& dir);
std::string findMtl(const std::string& dir);
std::vector<std::string> findPngs(const std::string& dir);
std::string getPath(const std::string &path);
bool existPath(const std::string &path);
#endif //FILEUTILS_H
