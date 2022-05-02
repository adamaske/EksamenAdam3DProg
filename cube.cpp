#include "cube.h"
#include "vertex.h"
#include "shader.h"
#include "cubecollision.h"
Cube::Cube(Shader& shader) : VisualObject(shader)
{
    //1
    mVertices.push_back(Vertex(0.5,0.5,0.5,1,0,0));
    mVertices.push_back(Vertex(0.5,-0.5,0.5,1,0,0));
    mVertices.push_back(Vertex(-0.5,-0.5,0.5,1,0,0));

    mVertices.push_back(Vertex(-0.5,-0.5,0.5,0,1,0));
    mVertices.push_back(Vertex(-0.5,0.5,0.5,0,1,0));
    mVertices.push_back(Vertex(0.5,0.5,0.5,0,1,0));

    //2
    mVertices.push_back(Vertex(0.5,0.5,-0.5,0,0,1));
    mVertices.push_back(Vertex(0.5,-0.5,-0.5,0,0,1));
    mVertices.push_back(Vertex(-0.5,-0.5,-0.5,0,0,1));

    mVertices.push_back(Vertex(-0.5,-0.5,-0.5,1,1,0));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5,1,1,0));
    mVertices.push_back(Vertex(0.5,0.5,-0.5,1,1,0));

    //3
    mVertices.push_back(Vertex(0.5,0.5,0.5,1,0,1));
    mVertices.push_back(Vertex(0.5,-0.5,0.5,1,0,1));
    mVertices.push_back(Vertex(0.5,0.5,-0.5,1,0,1));

    mVertices.push_back(Vertex(0.5,-0.5,-0.5,0,1,1));
    mVertices.push_back(Vertex(0.5,-0.5,0.5,0,1,1));
    mVertices.push_back(Vertex(0.5,0.5,-0.5,0,1,1));

    //4
    mVertices.push_back(Vertex(-0.5,-0.5,-0.5,0.5,0.4,0.1));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5,0.5,0.4,0.1));
    mVertices.push_back(Vertex(-0.5,-0.5,0.5,0.5,0.4,0.1));

    mVertices.push_back(Vertex(-0.5,0.5,0.5,0.1,0.4,0.5));
    mVertices.push_back(Vertex(-0.5,0.5,-0.5,0.1,0.4,0.5));
    mVertices.push_back(Vertex(-0.5,-0.5,0.5,0.1,0.4,0.5));

    //Bottom
    mVertices.push_back(Vertex(0.5,-0.5,0.5,0.956,0.741,1));
    mVertices.push_back(Vertex(-0.5,-0.5,0.5,0.956,0.741,1));
    mVertices.push_back(Vertex(0.5,-0.5,-0.5,0.956,0.741,1));

    mVertices.push_back(Vertex(-0.5,-0.5,-0.5,0.956,1,0.741));
    mVertices.push_back(Vertex(-0.5,-0.5,0.5,0.956,1,0.741));
    mVertices.push_back(Vertex(0.5,-0.5,-0.5,0.956,1,0.741));

    //Top
    mVertices.push_back(Vertex(0.5,0.5,0.5,0.546,0.5454,0.23));
    mVertices.push_back(Vertex(-0.5,0.5,0.5,0.546,0.5454,0.23));
    mVertices.push_back(Vertex(0.5,0.5,-0.5,0.546,0.5454,0.23));

    mVertices.push_back(Vertex(-0.5,0.5,-0.5,0.98,0.34,0.23));
    mVertices.push_back(Vertex(-0.5,0.5,0.5,0.98,0.34,0.23));
    mVertices.push_back(Vertex(0.5,0.5,-0.5,0.98,0.34,0.23));

   //mCollision = new CubeCollision();
    mMatrix.setToIdentity();
}
void Cube::init() {

    VisualObject::init();
}

void Cube::draw()
{
    VisualObject::draw();
}


