#include "light.h"
#include "shader.h"
#include "vertex.h"
#include "texture.h"
Light::Light(Shader& shader) : VisualObject(shader)
{

    mVertices.insert( mVertices.end(),
    {
        Vertex{Vector3D(-0.25f, -0.25f,  0.25f),    Vector3D(0.8f, 0.8f, 0.3f),  std::pair<float, float>{0.f,  0.0f}},
        Vertex{Vector3D( 0.25f, -0.25f,  0.25f),    Vector3D(0.8f, 0.8f, 0.3f),  std::pair<float, float>{1.f,  0.0f}},
        Vertex{Vector3D( 0.0f,  0.25f,  0.0f),      Vector3D(0.8f, 0.8f, 0.3f),  std::pair<float, float>{0.5f, 0.5f}},
        Vertex{Vector3D( 0.0f, -0.25f, -0.25f),     Vector3D(0.8f, 0.8f, 0.3f),  std::pair<float, float>{0.5f, 0.5f}}
    });

    mIndices.insert( mIndices.end(),
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
    });

    mMatrix.setToIdentity();
}

void Light::init(){
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

void Light::draw(){
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

