#include "oobbcollision.h"

OOBBCollision::OOBBCollision(VisualObject* object) : CollisionShape(object)
{
    mCollisionMode = CollisionShapeMode::OOBB;
}
