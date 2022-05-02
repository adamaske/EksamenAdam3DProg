#ifndef BOMBERENEMY_H
#define BOMBERENEMY_H

#include "enemy.h"

class BomberEnemy : public Enemy
{
public:
    BomberEnemy(std::string fileName, Shader& shader, Texture* texture);
    virtual void Update() override;
};

#endif // BOMBERENEMY_H
