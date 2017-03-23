#include "weaponShip.h"
#include "../spaceObjects/bullet.h"

weaponShip::weaponShip(){}

bullet* weaponShip :: fire(int x, int y){
    return new bullet(x, y, -1, 2);
}

weaponShip::~weaponShip(){}
