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

    CollisionShapeMode mCollisionMode;

    bool CheckCollision(CollisionShape* col1, CollisionShape* col2);
    virtual bool Collide(CollisionShape* col);


    //For sphere kollisjon
    virtual void SetRadius(float amount);

    virtual void SetSize(float amount);

    virtual void SetCenter(QVector3D pos);

    float GetRadius(){return mRadius; };
    QVector3D GetCenter() {return mCenter;};

    void SetShouldCollide(bool value);
protected:
    bool bShouldCollide = false;
    VisualObject* mObject;
    QVector3D mCenter{};
    QVector3D mSize{};
    float mRadius = 1;
};

#endif // COLLISIONSHAPE_H
