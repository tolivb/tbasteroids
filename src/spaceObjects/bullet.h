#include <allegro.h>
#include "spaceObject.h"

#ifndef BULLET_H_
#define BULLET_H_

class bullet : public spaceObject{

public:
    bullet(int,int,int,int);
    void doAction(int);
    void draw(BITMAP*);
    virtual ~bullet();
private:
    void recalcCenter();
    int damage,x1,y1,x2,y2,speed,id;
    int direction;
};

#endif /*BULLET_H_*/
