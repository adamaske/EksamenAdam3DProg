#include "bomb.h"

Bomb::Bomb(std::string fileName, Shader& shader, Texture* texture, ObjectState state, CollisionShape* collision) : ObjMesh(fileName, shader, texture, state)
{
    mCollision = collision;
    mName = "Bomb";
}
