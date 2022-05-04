#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>

enum ObjectState{
    STATIC, DYNAMIC
};

class Vertex;
class Shader;
class Texture;
class CollisionShape;
class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
   VisualObject(Shader& shader, ObjectState state, CollisionShape* collision);
   VisualObject(Shader& shader, Texture* texture, ObjectState, CollisionShape* collision);
   ~VisualObject();
   virtual void init();
   virtual void draw();

   ObjectState mObjectState;

   virtual void Update();
   void UpdateTransform();

   QMatrix4x4 GetPosition() const;
   QVector3D GetPosition();
   std::pair<float, float> getPosition2D();
   void SetPosition(const QVector3D& position);
   void SetPosition(const QMatrix4x4& position);

   virtual void move(float dx, float dy, float dz);
   virtual void MoveForward(float amount);
   virtual void MoveRight(float amount);

   QVector3D GetScale() const;
   void SetScale(const QVector3D& scale);
   void SetScale(const QMatrix4x4& scale);

   QMatrix4x4 GetRotation() const;
   void SetRotation(const QVector3D& rotation);
   void rotate(float dx, float dy, float dz);
   void RotateRight(float amount);

   QVector3D GetForward();
   QVector3D GetRight();
protected:
   std::vector<Vertex> mVertices;
   std::vector<GLuint> mIndices;

   GLuint mEAB{0};
   GLuint mVAO{0};
   GLuint mVBO{0};

   GLint mTextureUniform{0};
   GLint mMatrixUniform{0};

   QMatrix4x4 mMatrix;
   QMatrix4x4 mPosition;
   QMatrix4x4 mRotation;
   QMatrix4x4 mScale;

   // Legger til rette for simulering
   QVector3D mVelocity;
   QVector3D mDirection;

   //Kollisjoner
   QVector3D mSize;
   //Referanse til en mCollision, som er en kollisjonshape
   CollisionShape* mCollision;
   bool bIsFrozen = false;
public:
   Shader &mShader;
   Texture* mTexture;

   QVector3D mObjectColor{1.0f, 1.0f,0.0f}; //Yellow?

   //Velocity
   QVector3D GetVelocity(){return mVelocity;};
   void SetVelocity(QVector3D newVelocity){ mVelocity = newVelocity;};

   //Collision stuff
   CollisionShape* GetCollisionShape(){return mCollision;};
   void SetCollisionShape(CollisionShape* coll);
   virtual bool Collide(CollisionShape* coll);
    void UpdateCollider();
   //navn
   std::string mName = "Object";
   std::string GetName(){return mName;};
   void SetName(std::string name);


   //Funksjon som stopper objektet
   void Freeze();
   //Lar det bevege seg igjen
   void Unfreeze();
   bool IsFrozen() {return bIsFrozen;};

};
#endif // VISUALOBJECT_H
