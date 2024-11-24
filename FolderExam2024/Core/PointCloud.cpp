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
#include <unordered_map>
#include "delaunator.hpp"




void PointCloud::CreatePlane(model& PointCloudModel, std::string path)
{
    friction = 0.001f;
    glm ::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);
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

            Vertex point(glm::vec3(xf, yf, zf), glm::vec3(0.f), glm::vec3(1.f), friction);
            // Update min/max bounds
            minPoint = glm::min(minPoint, point.XYZ);
            maxPoint = glm::max(maxPoint, point.XYZ);

            points.push_back(point);
        }
        count++;
    }

    file.close();
    for (auto &point : points) {
        point.XYZ -= minPoint;
        point.XYZ *= 0.1f;
        point.XYZ.x *= -1;
        if (point.XYZ.x > -25.f) {
            friction = 10.f;
            color = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else
        {
            friction = 1.f;
            color = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        PointCloudModel.vertices.emplace_back(point.XYZ, glm::vec3(0.f), color, friction);
    }


    // Convert points to a format that Delaunator can use
    std::vector<double> coords;
    for (const auto& vertex : PointCloudModel.vertices) {
        coords.push_back(vertex.XYZ.x);
        coords.push_back(vertex.XYZ.z);
    }

    // Perform Delaunay triangulation
    delaunator::Delaunator d(coords);

    
    int amoutOfTriangles = 0;
    // Extract triangle indices
    for (size_t i = 0; i < d.triangles.size(); i += 3) {
        Triangle tri(d.triangles[i], d.triangles[i + 1], d.triangles[i + 2]);
        PointCloudModel.indices.push_back(tri);
        amoutOfTriangles++;
    }
    
    std::cout << "Number of points: " << count << std::endl;
    std::cout << "Number of triangles: " << amoutOfTriangles << std::endl;
    for (Triangle& index : PointCloudModel.indices)
    {
        glm::vec3 normal = glm::cross( PointCloudModel.vertices[index.B].XYZ - PointCloudModel.vertices[index.A].XYZ, PointCloudModel.vertices[index.C].XYZ - PointCloudModel.vertices[index.A].XYZ);

        PointCloudModel.vertices[index.A].Normals += glm::normalize(normal);
        PointCloudModel.vertices[index.B].Normals += glm::normalize(normal);
        PointCloudModel.vertices[index.C].Normals += glm::normalize(normal);
    }
    PointCloudModel.Bind();
}


