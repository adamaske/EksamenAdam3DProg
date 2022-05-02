#include "aabbcollision.h"

AABBCollision::AABBCollision(QVector3D pos, QVector3D size, VisualObject* object) : CollisionShape(object)
{
    mCollisionMode = CollisionShapeMode::AABB;
}

bool AABBCollision::Collide(CollisionShape *col){
    switch(col->mCollisionMode){
        case CollisionShapeMode::AABB :
            //Two aabb collision
            break;
        case CollisionShapeMode::OOBB :
            //AABB and OOBB collision
            break;
        case CollisionShapeMode::CUBE :
            //AABB and Cube collision
            break;
        case CollisionShapeMode::SPHERE :
            //AABB and Sphere collision
            break;
        default :
            //
            break;
    }

    return false;
}
