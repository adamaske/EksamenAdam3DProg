#ifndef BOMB_H
#define BOMB_H

#include "objmesh.h"
class SphereCollision;
class Bomb : public ObjMesh
{
public:
    Bomb(std::string fileName, Shader& shader, Texture* texture, ObjectState state, CollisionShape* collision);
    bool bDraw;
    bool CollidedWithBomb();
    virtual void draw() override;
};

#endif // BOMB_H
