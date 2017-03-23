#include "weapon.h"
#include "../spaceObjects/bullet.h"

#ifndef WEAPONSHIP_H_
#define WEAPONSHIP_H_


class weaponShip : public weapon
{

public:
    weaponShip();
    bullet* fire(int, int);
    virtual ~weaponShip();

};

#endif /*WEAPONSHIP_H_*/
