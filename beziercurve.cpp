#include "beziercurve.h"
#include "vertex.h"
BezierCurve::BezierCurve(std::vector<QVector3D> controlPoints, Shader& shader) : VisualObject(shader, ObjectState::STATIC)
{
    mControlPoints = controlPoints;
    //Create vertexs from control points
    for (auto it : mControlPoints) {
        mControlPointsVertices.push_back(Vertex(it.x(), it.y(), it.z(), 1.f, 1.f, 1.f));
    }
    //Visualpoint for displaying control points
    mControlPointVisual = new VisualPoint(mControlPointsVertices, shader);

    for (float t{0}; t <= 1.00f; t += 0.01f) {
        QVector3D point = EvaluateBezier(t);

        mVertices.push_back(Vertex(point.x(), point.y(), point.z()));
    }
}
void BezierCurve::init() {
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Vertex buffer object(VBO), holding vertices
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER,
        mVertices.size() * sizeof(Vertex),
        mVertices.data(),
        GL_STATIC_DRAW
    );

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    if (mControlPointVisual)
        mControlPointVisual->init();
}

void BezierCurve::draw() {
    initializeOpenGLFunctions();

    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());

    if (mControlPointVisual)
        mControlPointVisual->draw();
}

QVector3D BezierCurve::EvaluateBezier(float t)
{
    std::vector<QVector3D> temp;

    //Gets the control points
    for (int i = 0; i < mControlPoints.size(); i++) {
        temp.push_back(mControlPoints[i]);
    }
    for(int k = temp.size()-1; k > 0; k--)
    {
        for(int i = 0; i < k; i++)
            //Bezier algoritmen
            temp[i] = temp[i] * (1 - t) + temp[i + 1] * t;
    }
    return temp[0];
}
