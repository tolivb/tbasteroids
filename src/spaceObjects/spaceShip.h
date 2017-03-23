#include <allegro.h>
#include "spaceObject.h"
#include "../weapons/weapon.h"

#define SPACESHIP_WIDTH 80
#define SPACESHIP_HEIGHT 87
#define SPACESHIP_YOFFSET 10
#define SPACESHIP_SPEED 8

#ifndef SPACESHIP_H_
#define SPACESHIP_H_

class spaceShip : public spaceObject
{
public:
    spaceShip(int,int,int,int);
    void draw(BITMAP*);
    void doAction(int);
    virtual ~spaceShip();
private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void recalcCenter();
    int x1,y1,x2,y2;
    int speed;
    weapon* gun;
};

#endif /*SPACESHIP_H_*/
