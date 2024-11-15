#include "Model.h"

void model::Bind()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
        
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triangle)* indices.size(), indices.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
        
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);     glBindVertexArray(0);

    float MinX =  FLT_MAX;
    float MaxX = -FLT_MAX;
    float MinY =  FLT_MAX;
    float MaxY = -FLT_MAX;
    float MinZ =  FLT_MAX;
    float MaxZ = -FLT_MAX;
    for (Vertex element : vertices)
    {
        MinX = glm::min( MinX, element.XYZ.x);
        MaxX = glm::max( MaxX, element.XYZ.x);
        MinY = glm::min( MinY, element.XYZ.y);
        MaxY = glm::max( MaxY, element.XYZ.y);
        MinZ = glm::min( MinZ, element.XYZ.z);
        MaxZ = glm::max( MaxZ, element.XYZ.z);
    }
    corners = {
        glm::vec3(MinX, MinY, MinZ),
        glm::vec3(MaxX, MinY, MinZ),
        glm::vec3(MaxX, MaxY, MinZ),
        glm::vec3(MinX, MaxY, MinZ),
        glm::vec3(MinX, MinY, MaxZ),
        glm::vec3(MaxX, MinY, MaxZ),
        glm::vec3(MaxX, MaxY, MaxZ),
        glm::vec3(MinX, MaxY, MaxZ)
    };
}

void model::CalculateMatrix()
{
    ModelMatrix  =glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, PlayerPos);
    ModelMatrix = glm::rotate(ModelMatrix,glm::radians(PlayerRotation.x),glm::vec3(1.f,0.f,0.f));
    ModelMatrix = glm::rotate(ModelMatrix,glm::radians(PlayerRotation.y),glm::vec3(0.f,1.f,0.f));
    ModelMatrix = glm::rotate(ModelMatrix,glm::radians(PlayerRotation.z),glm::vec3(0.f,0.f,1.f));
    ModelMatrix = glm::scale(ModelMatrix,PlayerScale);
}

void model::CalculateBoundingBox()
{
    BoundingBox.MinPos = glm::vec3(FLT_MAX);
    BoundingBox.MaxPos = glm::vec3(-FLT_MAX);
       
    for (glm::vec3 element : corners)
    {
        element = ModelMatrix * glm::vec4(element, 1.f);
        BoundingBox.MinPos = glm::min(BoundingBox.MinPos, element);
        BoundingBox.MaxPos = glm::max(BoundingBox.MaxPos, element);
    }
}

void model::DrawMesh(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);
        
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glBindVertexArray(VAO);

    if(isLine) {
        glDrawArrays(GL_LINES, 0, vertices.size());
        glBindVertexArray(0);

        return;
    }
    if (indices.empty())
    {   
        glDrawArrays(GL_POINTS, 0, vertices.size());
    }
    else
    {
        glDrawElements(GL_TRIANGLES, indices.size()* 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


        
}
