#include <allegro.h>
#include <list>
#include "spaceObjects/spaceObject.h"

#ifndef OBJECTSMANAGER_H_
#define OBJECTSMANAGER_H_

class objectsManager{
    
public:
    static objectsManager* getInstance();
    void setParams(BITMAP*, int, int);
    void init();
    void runObjectsLogic(int);
    void renderObjects();
    void restart();

    spaceObject* generateEnemy();
    spaceObject* generateAsteroid();
    spaceObject* spaceShip;
    std :: list<spaceObject*> asteroids;
    std :: list<spaceObject*> enemies;
    std :: list<spaceObject*> bulletsEnemy;
    std :: list<spaceObject*> bulletsShip;
    std :: list<spaceObject*> sobjects;

    ~objectsManager();
private:
    objectsManager();
    bool runAsteroidsLogic();
    bool runEnemiesLogic();
    bool runShipLogic(int);
    static objectsManager* omanager;
    BITMAP *battlefield;
    int maxEnemies;
    int maxAsteroids;
};
#endif /*OBJECTSMANAGER_H_*/


