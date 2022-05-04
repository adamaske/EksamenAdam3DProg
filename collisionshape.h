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
    //Konstruer med modus og objekt
    CollisionShape(VisualObject* object, CollisionShapeMode mode);

    virtual bool Collide(CollisionShape* col);

    void UpdateCollider();
    //For sphere kollisjon
    virtual void SetRadius(float amount);

    virtual void SetSize(float amount);

    virtual void SetCenter(QVector3D pos);

    float GetRadius(){return mRadius; };

    QVector3D GetCenter() {return mCenter;};
    //Endre om kollisjonen får lov til å kollidere
    void SetShouldCollide(bool value);
protected:
    //Hvilket primitv skal det kollidere som
    CollisionShapeMode mCollisionMode;
    //Hvis en kollisjon skal bli godtatt, deaktiveres om objektet bare kan kollidere en gang
    bool bShouldCollide = true;
    //Referanse til objektet denne kollisjonen sitter på
    VisualObject* mObject;
    //Senter av kollisjonen,
    QVector3D mCenter{};
    //Størrelsen, ikke brukt
    QVector3D mSize{};
    //Hvis dette er en sphere kollisjon, så trenger den radius
    float mRadius = 1;
};

#endif // COLLISIONSHAPE_H
