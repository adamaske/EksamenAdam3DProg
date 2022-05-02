#include "trophy.h"
#include "shader.h"
#include "vector3d.h"
#include "vertex.h"
#include "texture.h"
Trophy::Trophy(std::string fileName, Shader& shader, Texture* texture, ObjectState state, CollisionShape* coll, TrophyColor color) : ObjMesh(fileName ,shader, texture, state, coll)
{
    //readFile(fileName);
    mMatrix.setToIdentity();
}

void Trophy::init(){
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, mTexture->id());
    if(mTexture){
        mTextureUniform = glGetUniformLocation(mShader.getProgram(), "textureSampler");
    }
    glBindVertexArray(0);
}

void Trophy::draw(){
    if(!bDraw){
        return;
    }
    if(mTexture){
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, mTexture->id());
       glUniform1i(mTextureUniform, 0);
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

bool Trophy::PickupTrophy(){
    if(bDraw){
        bDraw = false;
        mObjectState = ObjectState::STATIC;
        return true;
    }else{
        return false;
    }
}
