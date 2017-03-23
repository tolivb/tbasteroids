#include "spaceObjectsFactory.h"
#include "spaceObject.h"
#include "enemyShip.h"
#include "asteroid.h"
#include "bullet.h"
#include "spaceShip.h"
#include "../constants.h"

spaceObjectsFactory::spaceObjectsFactory()
{
}

spaceObject* spaceObjectsFactory :: getObject(int x, int y, int maxX, int maxY, int type=1) {
    switch (type) {
        case Constants::OBJECT_SHIP:
            return new spaceShip(x, y, maxX, maxY);
        case Constants::OBJECT_ASTEROID:
            return new asteroid(x, y, maxX, maxY);
        case Constants::OBJECT_ENEMY :
            return new enemyShip(x, y, maxX, maxY);
        default: 
            return new bullet(x, y, 1, 1);
    }
}

spaceObject* spaceObjectsFactory :: getEnemy(int x, int y, int maxX, int maxY) {
    return spaceObjectsFactory::getObject(x, y, maxX, maxY, Constants::OBJECT_ENEMY);
}

spaceObject* spaceObjectsFactory :: getShip(int x, int y, int maxX, int maxY) {
    return spaceObjectsFactory::getObject(x, y, maxX, maxY, Constants::OBJECT_SHIP);
}

spaceObject* spaceObjectsFactory :: getAsteroid(int x, int y, int maxX, int maxY) {
    return spaceObjectsFactory::getObject(x, y, maxX, maxY, Constants::OBJECT_ASTEROID);
}

spaceObject* spaceObjectsFactory :: getBullet(int x, int y, int maxX, int maxY) {
    return spaceObjectsFactory::getObject(x, y, maxX, maxY, 1);
}


spaceObjectsFactory::~spaceObjectsFactory()
{
}
