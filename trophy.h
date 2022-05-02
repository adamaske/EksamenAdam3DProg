#ifndef TROPHY_H
#define TROPHY_H

#include "objmesh.h"

class Trophy : public ObjMesh
{
public:
    Trophy(std::string fileName, Shader& shader, Texture* texture, ObjectState state);
    void init() override;
    void draw() override;

    void PickupTrophy();
    bool bDraw = true;
};

#endif // TROPHY_H
