#ifndef ENEMY_H
#define ENEMY_H

#include "objmesh.h"
class Enemy : public ObjMesh
{
public:
    Enemy(std::string fileName, Shader& shader, Texture* texture);

};

#endif // ENEMY_H
