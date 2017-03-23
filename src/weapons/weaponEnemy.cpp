#include "weaponEnemy.h"
#include "../spaceObjects/bullet.h"

weaponEnemy::weaponEnemy(){}

bullet* weaponEnemy :: fire(int x, int y){
    return new bullet(x, y, 1, 1);
}

weaponEnemy::~weaponEnemy(){}
