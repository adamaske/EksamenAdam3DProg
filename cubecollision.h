#ifndef CUBECOLLISION_H
#define CUBECOLLISION_H

#include "collisionshape.h"
class CubeCollision : public CollisionShape
{
public:
    CubeCollision(QVector3D pos, QVector3D size, VisualObject* object);


    virtual bool Collide(CollisionShape* col) override;
};

#endif // CUBECOLLISION_H
