#include "spherecollision.h"

SphereCollision::SphereCollision(QVector3D center, float radius, VisualObject* object) : CollisionShape(object)
{
    mCollisionMode = CollisionShapeMode::SPHERE;
    mCenter = center;
    mRadius = radius;
}



