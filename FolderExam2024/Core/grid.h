#pragma once
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <cmath>
#include "Model.h"
class grid
{
public:
    struct Cell {
        std::vector<Triangle> triangles;
    };

    grid(const glm::vec3& minPoint, const glm::vec3& maxPoint, float cellSize);
    void addTriangle(const Triangle& triangle, const std::vector<Vertex>& vertices);
    std::vector<Triangle> getTrianglesInCell(const glm::vec3& position) const;

private:
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
    float cellSize;
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, Cell>>> cells;

    glm::ivec3 getCellIndex(const glm::vec3& position) const;
};
