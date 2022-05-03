#ifndef SPHERECOLLISION_H
#define SPHERECOLLISION_H

#include "collisionshape.h"

class SphereCollision : public CollisionShape
{
public:
    SphereCollision(QVector3D center, float radius, VisualObject* object);
};

#endif // SPHERECOLLISION_H
