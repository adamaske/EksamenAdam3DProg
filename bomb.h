#ifndef BOMB_H
#define BOMB_H

#include "objmesh.h"
class SphereCollision;
class Bomb : public ObjMesh
{
public:
    Bomb(std::string fileName, Shader& shader, Texture* texture, ObjectState state, CollisionShape* collision);

};

#endif // BOMB_H
