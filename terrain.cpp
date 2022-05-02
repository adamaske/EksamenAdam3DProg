#include "terrain.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "vertex.h"
#include "texture.h"
Terrain::Terrain(Shader& shader, Texture* texture) : VisualObject(shader)
{

    //Array of pixel data, load function sets the width, height and channels read from the image
    unsigned char *data = stbi_load("../EksamenAdam3DProg/EksamenHeightmap.jpg",
                                    &width, &height, &nChannels,
                                    0);
    //Taken from https://learnopengl.com/Guest-Articles/2021/Tessellation/Height-map
    //data contains width * height * nChannels elements
    // vertex generation
    // apply a scale+shift to the height data
    float yScale = 64.0f / 256.0f, yShift = 32.0f;
    unsigned char* texel;
    unsigned char y;
    for( int i = 0; i < height; i++)
    {
        for( int j = 0; j < width; j++)
        {
            // retrieve texel for (i,j) tex coord
            texel = data + (j + width * i) * nChannels;
            // raw height at coordinate
            y = texel[0];

            // vertex
            //X vertex starts at the very right, then adds i to what posistion the loop is currently on
            vertices.push_back( -height/2.0f + i );        // v.x
            //the y value is the texel[0] and transformed with scale and shift, these can be changed
            vertices.push_back( (int)y * yScale - yShift); // v.y
            //Y does the same as x
            vertices.push_back( -width/2.0f + j );        // v.z
        }
    }


    //The y values are now read and the image can be removed from memory
    stbi_image_free(data);

    //Create mesh from these vertices
    for(long int i = 0; i < vertices.size(); i+=3){
        mVertices.push_back(Vertex(vertices[i],vertices[i+1],vertices[i+2]));
    }
    // index generation
    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < height-1; i++)       // for each row a.k.a. each strip
    {
        for(unsigned int j = 0; j < width; j++)      // for each column
        {
            for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                mIndices.push_back(j + width * (i + k));
            }
        }
    }
    NUM_STRIPS = height-1;
    NUM_VERTS_PER_STRIP = width*2;
    mTexture = texture;
    mMatrix.setToIdentity();
}

void Terrain::init(){
    initializeOpenGLFunctions();
    //Get the model matrix from shader
    mMatrixUniform =  glGetUniformLocation(mShader.getProgram(), "mMatrix");
    //Vertex array object-VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex buffer object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    //Verts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //uvs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Element array buffer - EAB
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}
void Terrain::draw(){
    //Bruk tekstur hvis det ikke er nullptr
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
    for(unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
    {
    glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)* NUM_VERTS_PER_STRIP * strip));
    }
    glBindVertexArray(0);
}

float Terrain::GetHeight(QVector3D pos){
    //qDebug() << height << width;
    //Find what width and height the pos is at
    //and compare to the vertex data of the terrain
    //Converts the float pos.x() to an in, so it can be used to refrence index'es form the vertex array
    float x = pos.x();
    float z = pos.z();
    qDebug()<< "Player pos: " << x << ", " << z;
    //First vertex of the triangle
    Vertex v1 = mVertices[(x*(width))+z];
    //Get the vertex to the right of v1
    Vertex v2 = mVertices[((x)*width)+z+1];
    //Get the vertex beneath v1
    Vertex v3 = mVertices[((x+1)*width)+z+1];
    //Vertex under and right of v1
    Vertex v4 = mVertices[((x+1)*width)+z];

    QVector3D bary = GetBaycentric(pos, QVector3D(v1.x, v1.y, v1.z), QVector3D(v2.x, v2.y, v2.z), QVector3D(v3.x,v3.y, v3.z));
    if((bary.x() + bary.y() + bary.z()) != 1){
        float a = v1.y * bary.x();
        float b = v2.y * bary.y();
        float c = v3.y * bary.z();
        float height = a + b + c;
        qDebug() << "Using top vertex:  " << height;
        return height + 32;
    }else{
              bary = GetBaycentric(pos, QVector3D(v1.x, v1.y, v1.z), QVector3D(v2.x, v2.y, v2.z), QVector3D(v4.x, v4.y, v4.z));
        float a = v1.y * bary.x();
        float b = v2.y * bary.y();
        float c = v4.y * bary.z();
        float height = a + b + c;
        qDebug() << "Using bottom vertex: " << height;
        return height + 32;
    }

}

QVector3D Terrain::GetBaycentric(QVector3D point, QVector3D a,  QVector3D b, QVector3D c){
    double u = 0, v = 0, w = 0;
    //Finner x
    QVector3D x1 = b - a;
    QVector3D x2 = c - a;
    x1.setY(0);
    x2.setY(0);
    QVector3D normal = QVector3D::crossProduct(x1, x2);
    float x = normal.length();
    //Regner kryssprodukt av p og q, og deler på x
    //Bruker a - point, b - point og c - point og kryssproduktet av kombinasjonen av de
    QVector3D p = b - point;
    QVector3D q = c - point;
    p.setY(0);
    q.setY(0);
    //Kryss produkt for u, delt på x
    normal = QVector3D::crossProduct(p,q);
    u = normal.y() / x;

    p = c - point;
    q = a - point;
    p.setY(0);
    q.setY(0);
    //Kryss produkt for v, delt på x
    normal = QVector3D::crossProduct(p, q);
    v = normal.y() / x;

    p = a - point;
    q = b - point;
    p.setY(0);
    q.setY(0);
    //Kryssprodukt for w, delt på x
    normal = QVector3D::crossProduct(p, q);
    w = normal.y() / x;
    qDebug() << u << v << w;
    //Finn om punkten er innenfor eller utenfor triangelt
    return QVector3D(u, v, w);
}
