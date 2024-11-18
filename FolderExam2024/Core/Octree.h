#pragma once
#include <vector>
#include <glm/glm.hpp>

class Octree {
public:
    struct Node {
        glm::vec3 minBounds;
        glm::vec3 maxBounds;
        std::vector<int> pointIndices;
        Node* children[8] = { nullptr };

        Node(const glm::vec3& minB, const glm::vec3& maxB) : minBounds(minB), maxBounds(maxB) {}
    };

    Octree(const glm::vec3& minBounds, const glm::vec3& maxBounds, int maxPointsPerNode = 10)
        : root(new Node(minBounds, maxBounds)), maxPointsPerNode(maxPointsPerNode) {}

    ~Octree() { deleteNode(root); }

    void insert(int pointIndex, const glm::vec3& point);

    void getIndices(std::vector<int>& indices) const;

private:
    Node* root;
    int maxPointsPerNode;

    void insert(Node* node, int pointIndex, const glm::vec3& point) ;

    void subdivide(Node* node);

    bool isPointInBounds(const glm::vec3& point, const glm::vec3& minBounds, const glm::vec3& maxBounds) const;
    void getIndices(const Node* node, std::vector<int>& indices) const;

    void deleteNode(Node* node);
        
};