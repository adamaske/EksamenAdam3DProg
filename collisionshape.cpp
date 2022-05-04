#include "collisionshape.h"
#include "QVector2D"

CollisionShape::CollisionShape(VisualObject* object, CollisionShapeMode mode) : mObject(object)
{
    mCollisionMode = mode;
}

bool CollisionShape::Collide(CollisionShape *col){
    if(!bShouldCollide){
        return false;
    }
    //Pass på at kollisjonen finnes
    if(col){
        //Oppdater den andre kolliderern
        col->UpdateCollider();
        //Oppdater denne kollideren
        UpdateCollider();
        //Sjekk om dette er en sphere)
        if(mCollisionMode == CollisionShapeMode::SPHERE){
            //Gjør sphere til sphere om begge er spheres
            if(col->mCollisionMode == CollisionShapeMode::SPHERE){
                //Forskjellen mellom de sentrene
                QVector3D difference = GetCenter() - col->GetCenter();
                //Lengden på denne vektoren er avstanden mellom punktene
                float length = difference.length();
                //Hvis avstanden er mindre enn de to radiusene sammenlagt, så kolliderer objetkene
                if(length < (GetRadius()+col->GetRadius())){
                    return true;
                }else{
                    return false;
                }
            }
        }

        if(mCollisionMode == CollisionShapeMode::AABB){

        }

        if(mCollisionMode == CollisionShapeMode::SPHERE){

        }

        if(mCollisionMode == CollisionShapeMode::SPHERE){

        }

        if(mCollisionMode == CollisionShapeMode::SPHERE){

        }
    }

    return false;
}

void CollisionShape::UpdateCollider()
{
    SetCenter(mObject->GetPosition());
    SetRadius(2);
}

void CollisionShape::SetRadius(float amount)
{
    mRadius = amount;
}

void CollisionShape::SetSize(float amount)
{
    //epic fail
}

void CollisionShape::SetCenter(QVector3D pos)
{
    mCenter = pos;
}

void CollisionShape::SetShouldCollide(bool value)
{
    bShouldCollide = value;
}
