#include "spherecollision.h"

SphereCollision::SphereCollision(QVector3D center, float radius, VisualObject* object) : CollisionShape(object), mCenter(center), mRadius(radius)
{
    mCollisionMode = CollisionShapeMode::SPHERE;
}

bool SphereCollision::Collide(CollisionShape* col){

    return false;
}
