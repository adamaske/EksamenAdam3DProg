#include "cubecollision.h"

CubeCollision::CubeCollision(QVector3D pos, QVector3D size, VisualObject* object) : CollisionShape(object)
{
    mCollisionMode = CollisionShapeMode::CUBE;
}

bool CubeCollision::Collide(CollisionShape *col){

    return false;
}
