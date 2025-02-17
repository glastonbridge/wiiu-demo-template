#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

int LoadUFBX(const std::string &path, const char *objectName,
             std::vector<float> &vertices, std::vector<float> &texcoords,
             std::vector<float> &normals, std::vector<float> &boneIndices,
             std::vector<float> &boneWeights,
             std::vector<std::vector<glm::mat4>>
                 &animFrames // nested: frame, bone, matrix
);

int LoadTypeUFBX(const std::string &path,
                 std::vector<std::vector<float>> &vertices,
                 std::vector<std::vector<float>> &normals);