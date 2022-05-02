#ifndef OOBBCOLLISION_H
#define OOBBCOLLISION_H

#include "collisionshape.h"

class OOBBCollision : public CollisionShape
{
public:
    OOBBCollision(VisualObject* object);
};

#endif // OOBBCOLLISION_H
