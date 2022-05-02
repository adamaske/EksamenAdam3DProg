#ifndef AABBCOLLISION_H
#define AABBCOLLISION_H

#include "collisionshape.h"

class AABBCollision : public CollisionShape
{
public:
    AABBCollision(QVector3D pos, QVector3D size, VisualObject* object);

    virtual bool Collide(CollisionShape* col) override;
};

#endif // AABBCOLLISION_H
