#include "weapon.h"
#include "../spaceObjects/bullet.h"

#ifndef WEAPONENEMY_H_
#define WEAPONENEMY_H_


class weaponEnemy : public weapon
{

public:
    weaponEnemy();
    bullet* fire(int, int);
    virtual ~weaponEnemy();
};

#endif /*WEAPON1_H_*/
