#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include "QVector3D"
#include "visualobject.h"
enum CollisionShapeMode{
    CUBE, SPHERE, AABB, OOBB, KDOP
};

class CollisionShape
{
public:
    CollisionShape(VisualObject* object);
    VisualObject* mObject;
    CollisionShapeMode mCollisionMode;

    bool CheckCollision(CollisionShape* col1, CollisionShape* col2);
    virtual bool Collide(CollisionShape* col);

    QVector3D mPos{};
};

#endif // COLLISIONSHAPE_H
