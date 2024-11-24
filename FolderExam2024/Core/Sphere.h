#pragma once
#include "Model.h"

class Sphere{
    
public:
   
    float radius = 0.25f;
 void SubDivide(int A,int B, int C, int NumOfDiv, model& SphereModel)
{
    
    if(NumOfDiv > 0)
    {
        glm::vec3 v1 =glm::normalize(SphereModel.vertices[A].XYZ + SphereModel.vertices[B].XYZ);
        glm::vec3 v2 =glm::normalize(SphereModel.vertices[A].XYZ+SphereModel.vertices[C].XYZ);
        glm::vec3 v3 =glm::normalize(SphereModel.vertices[C].XYZ+SphereModel.vertices[B].XYZ );
        

        int index1 =SphereModel.vertices.size(); 
        SphereModel.vertices.emplace_back(v1,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f),0);
        int index2 = SphereModel.vertices.size();
        SphereModel.vertices.emplace_back(v2,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f),0);
        int index3 = SphereModel.vertices.size();
        SphereModel.vertices.emplace_back(v3,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f),0);
        
        SubDivide(A,index1,index2, NumOfDiv -1, SphereModel);
        SubDivide(C,index2,index3, NumOfDiv -1,SphereModel);
        SubDivide(B,index3,index1, NumOfDiv -1,SphereModel);
        SubDivide(index3,index2,index1, NumOfDiv -1,SphereModel);
    }
    else
    {
        SphereModel.indices.emplace_back(A,B,C);
    }
}


inline void CreateSphere(model& SphereModel)
{
    SphereModel.vertices.emplace_back(glm::vec3(0.f,0.f,1.f), glm::vec3(0.f), glm::vec3(0.6f),0);
    SphereModel.vertices.emplace_back(glm::vec3(1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f),0);
    SphereModel.vertices.emplace_back(glm::vec3(0.f,1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f),0);
    SphereModel.vertices.emplace_back(glm::vec3(-1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f),0);
    SphereModel.vertices.emplace_back(glm::vec3(0.f,-1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f),0);
    SphereModel.vertices.emplace_back(glm::vec3(0.f,0.f,-1.f), glm::vec3(0.f), glm::vec3(0.6f),0);

    SubDivide(0,1,2,4, SphereModel);
    SubDivide(0,2,3,4, SphereModel);
    SubDivide(0,3,4,4, SphereModel);
    SubDivide(0,4,1,4, SphereModel);
    SubDivide(5,2,1,4, SphereModel);
    SubDivide(5,3,2,4, SphereModel);
    SubDivide(5,4,3,4, SphereModel);
    SubDivide(5,1,4,4, SphereModel);


    for (Triangle& index : SphereModel.indices)
    {
        glm::vec3 normal = glm::cross(SphereModel.vertices[index.B].XYZ - SphereModel.vertices[index.A].XYZ, SphereModel.vertices[index.C].XYZ - SphereModel.vertices[index.A].XYZ);

        SphereModel.vertices[index.A].Normals += glm::normalize(normal);
        SphereModel.vertices[index.B].Normals += glm::normalize(normal);
        SphereModel.vertices[index.C].Normals += glm::normalize(normal);
    }
    
SphereModel.Bind();
    

    //Not the best way to scale, but having problems doing this during creation
     
    SphereModel.PlayerScale = glm::vec3(0.5f);
}

void Move(model& SphereModel, float deltatime, float gravity, float friction, float mass)
{
    SphereModel.PlayerPos += SphereModel.Velocity * deltatime;
    SphereModel.Velocity.y -= (gravity / mass) * deltatime;

    // Apply friction to the horizontal components of the velocity
    SphereModel.Velocity.x *= (1.0f - (friction / mass) * deltatime);
    SphereModel.Velocity.z *= (1.0f - (friction / mass) * deltatime);
}
};