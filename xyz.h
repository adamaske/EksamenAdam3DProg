#ifndef XYZ_H
#define XYZ_H

#include "visualobject.h"

class XYZ : public VisualObject
{
public:
    XYZ(Shader& shader, ObjectState state);

    void init() override;
    void draw() override;
};

#endif // XYZ_H
