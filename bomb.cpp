#include "bomb.h"
#include "texture.h"
#include "shader.h"
#include "collisionshape.h"
Bomb::Bomb(std::string fileName, Shader& shader, Texture* texture, ObjectState state, CollisionShape* collision) : ObjMesh(fileName, shader, texture, state)
{
    mCollision = collision;

    mName = "Bomb";
}
void Bomb::draw()
{
    if(!bDraw){
        return;
    }
    if(mTexture){
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, mTexture->id());
       glUniform1i(mTextureUniform, 0);
    }else{
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //use my shader
    glUseProgram(mShader.getProgram());
    //Send my model matrix
    mShader.SetUniformMatrix4fv(mMatrix, "mMatrix");
    //Draw object
    glBindVertexArray( mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

bool Bomb::CollidedWithBomb()
{
    if(!bDraw){
        return false;
    }
    bDraw = false;
    mCollision->SetShouldCollide(false);
    return true;
}

