#pragma once
#include "Model.h"

class Collision
{
public:
    /// collisjon mellom alle spheres i prosjektet
    /// @param sphere_models 
    void SphereSphereCollision(std::vector<model*> sphere_models)
    {
        float radius = 0.5f;

        for (int i = 0; i < sphere_models.size() - 1; i++)
        {
            for (int j = i + 1; j < sphere_models.size(); j++)
            {
              
                glm::vec3 distanceVector = sphere_models[i]->PlayerPos - sphere_models[j]->PlayerPos;
                float distanceSquared = glm::dot(distanceVector, distanceVector);
                float radiusSum = 2.0f * radius;
                float radiusSumSquared = radiusSum * radiusSum;

                if (distanceSquared < radiusSumSquared)
                {
                    glm::vec3 collisionNormal = glm::normalize(distanceVector);

                    glm::vec3 relativeVelocity = sphere_models[i]->Velocity - sphere_models[j]->Velocity;
                    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
                    
                    float e = -0.3f; 
                    
                    float impulseMagnitude = (1 + e) * velocityAlongNormal;
                
                    
                    sphere_models[i]->Velocity -= impulseMagnitude * collisionNormal;
                    sphere_models[j]->Velocity += impulseMagnitude * collisionNormal;

                
                    float overlap = radiusSum - sqrt(distanceSquared);
                    glm::vec3 separation = collisionNormal * (overlap / 2.0f);
                    sphere_models[i]->PlayerPos += separation;
                    sphere_models[j]->PlayerPos -= separation;

                }
            }
        }
    }

/// collision sjekk mellom Sphere og Plane
/// @param PCloud, sphere_models
    
    /// collision sjekk mellom Sphere og AABB
    /// @param sphere_models 
    /// @param other_models 
    void SphereBoxCollision(const std::vector<model*>& sphere_models, const std::vector<model*>& other_models)
    {
        float radius = 0.5f;
        
        for (const auto& sphereModel : sphere_models)
        {
            for (const auto& boxModel : other_models)
            {
              
                glm::vec3 closestPoint = glm::clamp(
                    sphereModel->PlayerPos,
                    boxModel->BoundingBox.MinPos,
                    boxModel->BoundingBox.MaxPos
                );
                
                glm::vec3 distanceVector = sphereModel->PlayerPos - closestPoint;
                float distanceSquared = glm::dot(distanceVector, distanceVector);
                float radiusSquared = radius * radius;

                
                if (distanceSquared < radiusSquared)
                {
                    glm::vec3 collisionNormal = glm::normalize(distanceVector);
                    sphereModel->Velocity = glm::reflect(sphereModel->Velocity, collisionNormal);
                  //  std::cout << "BallBoxCollision" << '\n';
                }
            }
        }
    }


    /// En gammel collsison check som er basert AABB collision (ikke i bruk)
    /// @param AllModels 
    void CollisionCheck(std::vector<model*> AllModels)
    {
        glm::vec3 ClampedNormal = glm::vec3(0.f);
        for (int i = 0; i < AllModels.size() -1; i++)
        {
            for (int j = i +1; j < AllModels.size(); j++)
            if (aabb::AABBIntersect(AllModels[i]->BoundingBox,AllModels[j]->BoundingBox))
            {
                ClampedNormal = glm::clamp(glm::vec3(0.f),AllModels[j]->PlayerPos, AllModels[i]->PlayerPos);
                ClampedNormal = glm::normalize(ClampedNormal);
                std::cout<<"Collision"<<'\n';
                AllModels[i]->PlayerPos = AllModels[i]->PlayerPos;
                AllModels[i]->Velocity = glm::reflect(AllModels[i]->Velocity, ClampedNormal);
            }
           
        }
    }

  
};