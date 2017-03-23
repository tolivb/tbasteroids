#include <allegro.h>
#include "spaceObject.h"

#ifndef ASTEROID_H_
#define ASTEROID_H_
class asteroid : public spaceObject{

public:
    asteroid(int, int, int, int);
    void doAction(int);
    void draw(BITMAP*);
    virtual ~asteroid();
private:
    void recalcCenter();
    void move();
    void bounce();
    int x1,y1,x2,y2;
    int angle;
    int xd,yd;
    int speed;
};

#endif /*ASTEROID_H_*/
