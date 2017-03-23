#include "../spaceObjects/bullet.h"
#ifndef WEAPON_H_
#define WEAPON_H_

class weapon{

public:
    weapon();
    virtual bullet* fire(int,int)=0;
    virtual ~weapon();
};

#endif /*WEAPON_H_*/
