#include "collisionshape.h"
#include "QVector2D"
#include "spherecollision.h"
#include "cubecollision.h"
CollisionShape::CollisionShape(VisualObject* object) : mObject(object)
{

}

bool CollisionShape::CheckCollision(CollisionShape* col1, CollisionShape* col2){
//    QVector3D pos1;
//    QVector3D pos2;
//    QVector3D size1;
//    QVector3D size2 ;
//    if(col1->mCollisionMode == CollisionShapeMode::CUBE && col2->mCollisionMode == CollisionShapeMode::CUBE){
//        //Cube collisions

//        // collision x-axis?
//        bool collisionX = pos1.x() + size1.x() >= pos2.x() &&
//            pos2.x() + size2.x() >= pos1.x();
//        // collision y-axis?
//        bool collisionY = pos1.y() + size1.y() >= pos2.y() &&
//            pos2.y() + size2.y() >= pos1.y();
//        // collision only if on both axes
//        return collisionX && collisionY;
//    }
//    if((col1->mCollisionMode == CollisionShapeMode::CUBE && col2->mCollisionMode == CollisionShapeMode::SPHERE) ||
//            (col1->mCollisionMode == CollisionShapeMode::SPHERE && col2->mCollisionMode == CollisionShapeMode::SPHERE)){
//        //Cube and sphere collision
//        SphereCollision* sphere;
//        CollisionShape* cube;
//        if(col1->mCollisionMode == CollisionShapeMode::SPHERE){
//            sphere = static_cast<SphereCollision*>(col1);
//            cube = static_cast<CubeCollision*>(col2);
//        }else if(col2->mCollisionMode == CollisionShapeMode::SPHERE){
//            sphere = static_cast<SphereCollision*>(col2);
//            cube = static_cast<CubeCollision*>(col1);
//        }
//        QVector3D center = pos1;
//        QVector3D aabb_half_extents(size1 / 2);
//        QVector3D aabb_center(pos1 + aabb_half_extents);

//        QVector3D difference = center - aabb_center;


//    }
//    return false;
    return false;
}


bool CollisionShape::Collide(CollisionShape *col){
    if(col){
        if(mCollisionMode == CollisionShapeMode::SPHERE){

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

void CollisionShape::SetRadius(float amount)
{

}

void CollisionShape::SetSize(float amount)
{

}

void CollisionShape::SetCenter(QVector3D pos)
{

}
