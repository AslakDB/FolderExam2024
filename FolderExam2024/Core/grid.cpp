#include "grid.h"

grid::grid(const glm::vec3& minPoint, const glm::vec3& maxPoint, float cellSize) :
minPoint(minPoint), maxPoint(maxPoint), cellSize(cellSize){}

void grid::addTriangle(const Triangle& triangle, const std::vector<Vertex>& vertices)
{
        glm::vec3 v0 = vertices[triangle.A].XYZ;
        glm::vec3 v1 = vertices[triangle.B].XYZ;
        glm::vec3 v2 = vertices[triangle.C].XYZ;

        glm::vec3 min = glm::min(glm::min(v0, v1), v2);
        glm::vec3 max = glm::max(glm::max(v0, v1), v2);

        glm::ivec3 minIndex = getCellIndex(min);
        glm::ivec3 maxIndex = getCellIndex(max);

        for (int x = minIndex.x; x <= maxIndex.x; ++x) {
            for (int y = minIndex.y; y <= maxIndex.y; ++y) {
                for (int z = minIndex.z; z <= maxIndex.z; ++z) {
                    cells[x][y][z].triangles.push_back(triangle);
                }
            }
        }
}

std::vector<Triangle> grid::getTrianglesInCell(const glm::vec3& position) const
{
    glm::ivec3 index = getCellIndex(position);
    auto itX = cells.find(index.x);
    if (itX != cells.end()) {
        auto itY = itX->second.find(index.y);
        if (itY != itX->second.end()) {
            auto itZ = itY->second.find(index.z);
            if (itZ != itY->second.end()) {
                return itZ->second.triangles;
            }
        }
    }
    return {};  
}

glm::ivec3 grid::getCellIndex(const glm::vec3& position) const
{
    return glm::ivec3(
        static_cast<int>(std::floor((position.x - minPoint.x) / cellSize)),
        0,//static_cast<int>(std::floor((position.y - minPoint.y) / cellSize)),
        static_cast<int>(std::floor((position.z - minPoint.z) / cellSize)));
}
