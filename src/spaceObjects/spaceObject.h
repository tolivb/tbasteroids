#include <allegro.h>
#include "../collisions/ebox.h"

#ifndef SPACEOBJECT_H_
#define SPACEOBJECT_H_

class spaceObject
{
public :
    spaceObject(int, int, int, int);

    virtual void draw(BITMAP*)=0;
    virtual void doAction(int)=0;

    int getID();
    bool collidesWith(spaceObject*);
    bool getCollisionInProgress();
    virtual bool resurect();

    ECollide* collide;
    int cx,cy;
    BITMAP *obmp;

    virtual ~spaceObject();
protected: 
    int x, y, maxX, maxY;
    int id;
    int collisionObjectId;
    bool collisionInProgress;
};

#endif /*SPACEOBJECT_H_*/
