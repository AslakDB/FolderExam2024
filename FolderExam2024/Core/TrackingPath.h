#pragma once
#include "Model.h"

#include <vector>
struct TrackingPath
{
    std::vector<glm::vec3> points;
    void AddPoint(glm::vec3& point);
    float B( int i, int k, float t);
    std::vector<glm::vec3> CalculateBSpline(const std::vector<glm::vec3> &Points, int numPoints);
    void DrawPoints(model& pathModel,const  std::vector<glm::vec3>& Points);
    void CreateBSplinePath(model &pathModel,const std::vector<glm::vec3>& Points);
    
};
