#include "Octree.h"


void Octree::insert(int pointIndex, const glm::vec3& point)
{
    insert(root, pointIndex, point);
}

void Octree::getIndices(std::vector<int>& indices) const
{
    getIndices(root, indices);
}

void Octree::insert(Node* node, int pointIndex, const glm::vec3& point)
{
    if (!node) return;

    if (node->pointIndices.size() < maxPointsPerNode) {
        node->pointIndices.push_back(pointIndex);
    } else {
        if (!node->children[0]) subdivide(node);

        for (int i = 0; i < 8; ++i) {
            if (isPointInBounds(point, node->children[i]->minBounds, node->children[i]->maxBounds)) {
                insert(node->children[i], pointIndex, point);
                break;
            }
        }
    }
}

void Octree::subdivide(Node* node)
{
    glm::vec3 center = (node->minBounds + node->maxBounds) * 0.5f;
    glm::vec3 halfSize = (node->maxBounds - node->minBounds) * 0.5f;

    for (int i = 0; i < 8; ++i) {
        glm::vec3 offset(
            (i & 1) ? halfSize.x : 0,
            (i & 2) ? halfSize.y : 0,
            (i & 4) ? halfSize.z : 0
        );
        node->children[i] = new Node(node->minBounds + offset, center + offset);
    }
}

bool Octree::isPointInBounds(const glm::vec3& point, const glm::vec3& minBounds, const glm::vec3& maxBounds) const
{
    return (point.x >= minBounds.x && point.x <= maxBounds.x &&
                point.y >= minBounds.y && point.y <= maxBounds.y &&
                point.z >= minBounds.z && point.z <= maxBounds.z);
}

void Octree::getIndices(const Node* node, std::vector<int>& indices) const
{
    if (!node) return;

    if (node->pointIndices.size() >= 3) {
        indices.insert(indices.end(), node->pointIndices.begin(), node->pointIndices.end());
    }

    for (int i = 0; i < 8; ++i) {
        getIndices(node->children[i], indices);
    }
}

void Octree::deleteNode(Node* node)
{
    if (!node) return;

    for (int i = 0; i < 8; ++i) {
        deleteNode(node->children[i]);
    }

    delete node;

}
