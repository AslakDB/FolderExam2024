#pragma once
#include <glad/glad.h>

#include "AABB.h"
#include <vector>
#include "Vertex.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



class model
{
public:


unsigned int VBO, VAO, EBO;
    std::vector<Vertex> vertices;
    std::vector<Triangle> indices;
    std::vector<glm::vec3> corners;

    glm::mat4 ModelMatrix = glm::mat4(1.f);

    std::vector<glm::vec3> points;
    
    aabb BoundingBox;
    
    bool isLine = false;

    
    glm::vec3 PlayerPos = glm::vec3(0.f);
    glm::vec3 PlayerScale = glm::vec3(1.f);
    glm::vec3 PlayerRotation = glm::vec3(0.f);
    glm::vec3 Velocity = glm::vec3(0.f);

    
    
    auto Bind() -> void;
   
    glm::vec3 GetPlayerPos(){return PlayerPos;}
    glm::vec3 GetPlayerScale(){return PlayerScale;}
    glm::vec3 GetPlayerRotation(){return PlayerRotation;}

    void CalculateMatrix();

     void CalculateBoundingBox();

    
    void DrawMesh(unsigned int shaderProgram);
};


