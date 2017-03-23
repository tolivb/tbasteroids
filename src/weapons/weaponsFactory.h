#include "weapon.h"

#ifndef WEAPONSFACTORY_H_
#define WEAPONSFACTORY_H_

class weaponsFactory
{
    
public:
    weaponsFactory();
    static weapon* getWeaponEnemy();
    static weapon* getWeaponShip();
    virtual ~weaponsFactory();

};

#endif /*WEAPONSFACTORY_H_*/
