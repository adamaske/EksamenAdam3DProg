#include "bomberenemy.h"

BomberEnemy::BomberEnemy(std::string fileName, Shader& shader, Texture* texture) : Enemy(fileName, shader, texture)
{

}

void BomberEnemy::Update(){
    //Get current time
    QTime lastTime;
    QTime currentTime;
    currentTime = QTime::currentTime();
    if(lastTime.isValid()){
        if(currentTime.msec()-2 >= lastTime.msec()){
            //Drop bomb
            //mRenderWindow->SpawnObject(new Bomb(), GetPosition());
        }
    }
    lastTime = currentTime;
}
