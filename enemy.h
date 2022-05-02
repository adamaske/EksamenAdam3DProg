#ifndef ENEMY_H
#define ENEMY_H

#include "objmesh.h"

#include <QTime>
class Enemy : public ObjMesh
{
public:
    Enemy(std::string fileName, Shader& shader, Texture* texture, ObjectState state);
    virtual void Update() override;

};

#endif // ENEMY_H
