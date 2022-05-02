#ifndef LIGHT_H
#define LIGHT_H

#include "vector3d.h"
#include "visualobject.h"
class Shader;
class Light : public VisualObject
{
public:
    Light(Shader& shader);
    virtual void init() override;
    virtual void draw() override;

    GLfloat mAmbientStrength{0.1f};
    Vector3D mAbientColor{0.2f, 0.2f, 0.1f};

    GLfloat mLightStrenght{1.7f};
    Vector3D mLightColor{0.4f, 0.4f, 0.4f};    //Yellow

    GLfloat mSpecularStrenght{0.9f};
    GLint mSpecularExponent{4};
};

#endif // LIGHT_H
