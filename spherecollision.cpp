#include "spherecollision.h"

SphereCollision::SphereCollision(QVector3D center, float radius, VisualObject* object) : CollisionShape(object)
{
    mCollisionMode = CollisionShapeMode::SPHERE;
    mCenter = center;
    mRadius = radius;
}

bool SphereCollision::Collide(CollisionShape* col){
    //Begge er spheres, sjekk kollisjon
    if(col->mCollisionMode == mCollisionMode){
        QVector3D pos1 = mCenter;
        QVector3D pos2 = col->GetCenter();

        float size1 = mRadius;
        float size2 = col->GetRadius();

        QVector3D difference = pos1 - pos2;
        //Hvor langt er kulene fra hverandre
        float distance = difference.length();
        //De kolliderer dersom deres samlet radius er mindre enn distansen
        float radiusSum = size1 + size2;
        if(distance < radiusSum){
            return true;
        }else{
            return false;
        }
    }
    return false;
}

