#ifndef SPHERECOLLISION_H
#define SPHERECOLLISION_H

#include "collisionshape.h"

class SphereCollision : public CollisionShape
{
public:
    SphereCollision(QVector3D center, float radius, VisualObject* object);

    //Variables need for sphere collision
    QVector3D mCenter;
    float mRadius = 0;

    virtual bool Collide(CollisionShape* col) override;
};

#endif // SPHERECOLLISION_H
