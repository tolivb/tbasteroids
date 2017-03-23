#include "spaceObject.h"
#ifndef SPACEOBJECTSFACTORY_H_
#define SPACEOBJECTSFACTORY_H_

class spaceObjectsFactory
{
public:
    spaceObjectsFactory();
    static spaceObject* getObject(int,int,int,int,int);
    static spaceObject* getAsteroid(int,int,int,int);
    static spaceObject* getEnemy(int,int,int,int);
    static spaceObject* getShip(int,int,int,int);
    static spaceObject* getBullet(int,int,int,int);
    virtual ~spaceObjectsFactory();
};

#endif /*SPACEOBJECTSFACTORY_H_*/
