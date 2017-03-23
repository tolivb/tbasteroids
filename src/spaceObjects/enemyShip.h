#include <allegro.h>
#include "spaceObject.h"
#include "../weapons/weapon.h"

#ifndef ENEMYSHIP_H_
#define ENEMYSHIP_H_

class enemyShip : public spaceObject{
    
public:
    enemyShip(int, int, int, int);
    void draw(BITMAP*);
    void doAction(int);
    bool resurect();
    int getId();
    bool collidesWith(spaceObject*);
    virtual ~enemyShip();
private:
    void moveLeft(int pos=5);
    void moveRight(int pos=5);
    void moveUp(int pos=1);
    void moveDown(int pos=1);
    void recalcCenter();

    int x1, y1, x2, y2;
    int id;
    int speed;
    weapon* gun;
};

#endif /*ENEMYSHIP_H_*/
