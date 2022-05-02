#include "visualpoint.h"
#include "vertex.h"

VisualPoint::VisualPoint(const std::vector<Vertex>& v, Shader& shader) : VisualObject(shader, ObjectState::STATIC, nullptr)
{
    mVertices = v;
}

VisualPoint::~VisualPoint()
{
}

void VisualPoint::init()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<GLvoid*>(0));          // array buffer offset

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void VisualPoint::draw()
{
    initializeOpenGLFunctions();

    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, mVertices.size());
    //**********************************************
    //TIL BezierCurve
    //glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}
