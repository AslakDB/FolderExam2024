#pragma once
#include "Model.h"
#include <fstream>

class PointCloud
{
public:
    glm ::vec3 minPoint;
    glm::vec3 maxPoint;
    float friction = 0.f;
    void CreatePlane(model& PointCloudModel, std::string path);
    void CreateTextfile(std::vector<Vertex> points);
};
