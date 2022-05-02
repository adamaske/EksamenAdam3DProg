#ifndef OBJMESH_H
#define OBJMESH_H

#include "visualobject.h"
class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string fileName, Shader& shader, ObjectState state);
    ObjMesh(std::string fileName, Shader& shader, Texture* texture, ObjectState state);
    virtual void draw() override;
    virtual void init() override;
    virtual void Update() override;
protected:
    void readFile(std::string filePath);
    void writeFile(std::string filePath);
    GLuint mIBO{0};
};

#endif // OBJMESH_H
