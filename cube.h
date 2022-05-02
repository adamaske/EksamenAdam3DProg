#ifndef CUBE_H
#define CUBE_H


#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "visualobject.h"
class Shader;
class Cube : public VisualObject
{
public:
    Cube(Shader& shader);
    virtual void init() override;
    virtual void draw() override;
};

#endif // CUBE_H
