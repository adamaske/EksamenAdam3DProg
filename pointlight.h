#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"
class PointLight : public Light
{
public:
    PointLight(Shader& shader, int pointLightIndex);
    PointLight(Shader& shader, Texture* texture, int pointLightIndex);
    virtual void init() override;
    virtual void draw() override;

    int mPointLightIndex = 0;
};

#endif // POINTLIGHT_H
