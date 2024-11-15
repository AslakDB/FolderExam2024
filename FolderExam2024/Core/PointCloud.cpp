#include "PointCloud.h"
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cfloat>

void PointCloud::CreatePlane(model& PointCloudModel, std::string path)
{
    std::vector<Vertex> points;
    std::ifstream file(path);
    std::string line;

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << path << std::endl;
        return;
    }

    glm::vec3 minPoint(FLT_MAX);
    glm::vec3 maxPoint(-FLT_MAX);
    bool skipLine = true;
    int count = 0;
    while (std::getline(file, line)) {
        if (skipLine) {
            skipLine = !skipLine;
            continue;
        }

        float x, y, z;
        // Supports both space and tab separated values
        if (sscanf_s(line.c_str(), "%f %f %f", &x, &y, &z) == 3 || sscanf_s(line.c_str(), "%f\t%f\t%f", &x, &y, &z) == 3) {
            float xf = x;
            float yf = z;
            float zf = y;

            Vertex point(glm::vec3(xf, yf, zf), glm::vec3(0.f), glm::vec3(0.f));
            // Update min/max bounds
            minPoint = glm::min(minPoint, point.XYZ);
            maxPoint = glm::max(maxPoint, point.XYZ);

            points.push_back(point);
        }
        count++;
    }

    file.close();

    // Translate points to move the minimum point to the origin
    for (auto &point : points) {
        point.XYZ -= minPoint;
        point.XYZ *= 0.1f;
        point.XYZ.x *= -1;
        PointCloudModel.vertices.emplace_back(point.XYZ, glm::vec3(0.f), glm::vec3(0.f));
    }
    
    std::cout<<"Number of points: "<<count<<std::endl;
    PointCloudModel.Bind();
}