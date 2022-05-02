#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "visualobject.h"
#include "visualpoint.h"
#include <vector>

class BezierCurve : public VisualObject
{
public:
    BezierCurve(std::vector<QVector3D> controlPoints, Shader& shader);
    void init() override;
    void draw() override;
    int d = 3;
private:
    std::vector<QVector3D> mControlPoints;
    std::vector<Vertex> mControlPointsVertices;
    VisualPoint* mControlPointVisual;
public:
    QVector3D EvaluateBezier(float t);
};

#endif // BEZIERCURVE_H
