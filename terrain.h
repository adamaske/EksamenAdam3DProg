#ifndef TERRAIN_H
#define TERRAIN_H

#include "visualobject.h"
class Terrain : public VisualObject
{
public:
    Terrain(Shader& shader);
    void init() override;
    void draw() override;
    unsigned int NUM_STRIPS = 0;
    unsigned int NUM_VERTS_PER_STRIP = 0;
    float GetHeight(QVector3D pos);
    QVector3D GetBaycentric(QVector3D point, QVector3D a,  QVector3D b, QVector3D c);
    int width, height, nChannels;
     std::vector<float> vertices;
};

#endif // TERRAIN_H
