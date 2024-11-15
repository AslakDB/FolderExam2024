//
// Created by Aslak on 4/7/2024.
//
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef VERTEX_H
#define VERTEX_H
struct Vertex {
    glm::vec3 XYZ;
    glm::vec3 RGB;
    glm::vec3 Normals;
    Vertex(glm::vec3 XYZ,glm::vec3 Normals, glm::vec3 RGB) : XYZ(XYZ), RGB(RGB), Normals(Normals) {}
};

struct Triangle {
    unsigned int A;
    unsigned int B;
    unsigned int C;
    Triangle(unsigned int A, unsigned int B, unsigned int C) : A(A), B(B), C(C) {}
};
#endif //VERTEX_H
