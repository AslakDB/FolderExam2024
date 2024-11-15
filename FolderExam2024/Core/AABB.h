#pragma once
#include "glm/glm.hpp"


class aabb
{
public:

glm::vec3 MaxPos = glm::vec3(0);
glm::vec3 MinPos = glm::vec3(0);

    aabb() = default;
    aabb(glm::vec3 inMinPos, glm::vec3 inMaxPos)
    {
        MaxPos = inMaxPos;
        MinPos = inMinPos;
    }

/// AABB collision detection
/// @param Box1 
/// @param Box2 
/// @return 
static bool AABBIntersect(aabb Box1, aabb Box2)
    {
        if (Box1.MaxPos.x < Box2.MinPos.x || Box1.MinPos.x > Box2.MaxPos.x)
        {
            return false;
        }
        if (Box1.MaxPos.y < Box2.MinPos.y || Box1.MinPos.y > Box2.MaxPos.y)
        {
            return false;
        }
        if (Box1.MaxPos.z < Box2.MinPos.z || Box1.MinPos.z > Box2.MaxPos.z)
        {
            return false;
        }
        
        return true;
    }
};
