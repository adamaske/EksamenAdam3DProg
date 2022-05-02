#include "visualobject.h"
#include "vertex.h"
#include "shader.h"
#include "texture.h"
VisualObject::VisualObject(Shader& shader) : mShader{shader}
{
    mMatrix.setToIdentity();
}
VisualObject::VisualObject(Shader& shader, Texture* texture) : mShader{shader}, mTexture{texture}
{
    if(mTexture){
        //mTextureUniform = glGetUniformLocation(mShader.getProgram(), "textureSampler");
    }
}
VisualObject::~VisualObject() {
  /* glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );*/
}

void VisualObject::init(){
    initializeOpenGLFunctions();
    //Get the model matrix from shader
    mMatrixUniform =  glGetUniformLocation(mShader.getProgram(), "mMatrix");
    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void VisualObject::draw(){
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

}

void VisualObject::move(float dx, float dy, float dz){
    mPosition.translate(dx, dy, dz);
}

void VisualObject::move(float dt)
{
    float degrees = (180 * dt) / M_PI;
    mRotation.rotate(degrees, 0, 1, 0);

    //Slide
    QVector3D ds = mVelocity * dt;

    // mPosition = mPosition + ds;		// hvis mPosisjon er Vector3d
    mPosition.translate(ds.x(), ds.y(), ds.z());	// hvis mPosisjon er Matrix4x4

    // normalen kan generelt være en parameter inn
    QVector3D normal = QVector3D{ 0.0f, 1.0f, 0.0f };

    // bruker kryssprodukt for å finne rotasjonsvektor
    QVector3D rotation = QVector3D::crossProduct(normal, mVelocity);
    rotation.normalize();

    //mRotation.setToIdentity();
    // bruk formelen ds = r dt ==> dt = ds/r
    // for å finne ut hvor mye hjulet har rotert
    // og oppdater rotasjonsmatrisen
    // husk å starte med mRotation som identitetsmatrise

    mMatrix = mPosition * mRotation * mScale;
}

void VisualObject::rotate(float dx, float dy, float dz)
{
    mMatrix.rotate(dx, dy, dz);
}
std::pair<float, float> VisualObject::getPosition2D()
{
    auto col = mPosition.column(3);
    return std::pair<float, float>(col.x(), col.z());
}
void VisualObject::UpdateTransform()
{
    mMatrix = mPosition * mRotation * mScale;
}

QMatrix4x4 VisualObject::GetPosition() const
{
    return mPosition;
}
QVector3D VisualObject::GetPosition()
{
    return QVector3D(mPosition.column(3).x(),mPosition.column(3).y(),mPosition.column(3).z());
}

void VisualObject::SetPosition(const QVector3D& position)
{
    mPosition.setToIdentity();
    mPosition.translate(position);
}

void VisualObject::SetPosition(const QMatrix4x4& position)
{
    mPosition = position;
}

QVector3D VisualObject::GetScale() const
{
    return QVector3D(mScale.column(0).x(), mScale.column(1).y(), mScale.column(2).z());
}

void VisualObject::SetScale(const QVector3D& scale)
{
    mScale.setToIdentity();
    mScale.scale(scale);
}

void VisualObject::SetScale(const QMatrix4x4& scale)
{
    mScale = scale;
}

QMatrix4x4 VisualObject::GetRotation() const
{
    return mRotation;
}

void VisualObject::SetRotation(const QVector3D& rotation)
{
    mRotation.setToIdentity();
    mRotation.rotate(90.f, rotation);
}
