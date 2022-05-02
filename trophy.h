#ifndef TROPHY_H
#define TROPHY_H

#include "objmesh.h"

enum TrophyColor{BLUE, RED};
class Trophy : public ObjMesh
{
public:
    Trophy(std::string fileName, Shader& shader, Texture* texture, ObjectState state, CollisionShape* coll, TrophyColor color);
    void init() override;
    void draw() override;

    bool PickupTrophy();
    bool bDraw = true;

    TrophyColor mColor = TrophyColor::BLUE;
};

#endif // TROPHY_H
