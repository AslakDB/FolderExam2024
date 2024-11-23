#pragma once
#include "Model.h"
#include <fstream>

class PointCloud
{
public:
    glm ::vec3 minPoint;
    glm::vec3 maxPoint;
    void CreatePlane(model& PointCloudModel, std::string path);
};
