#ifndef VISUALSUN_H
#define VISUALSUN_H

#include "objmesh.h"

class VisualSun : public ObjMesh
{
public:
    VisualSun(std::string fileName, Shader& shader);
    float angle = 0;

};

#endif // VISUALSUN_H
