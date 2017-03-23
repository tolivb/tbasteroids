#include "weaponsFactory.h"
#include "weapon.h"
#include "weaponEnemy.h"
#include "weaponShip.h"

weaponsFactory::weaponsFactory(){}

weapon* weaponsFactory :: getWeaponEnemy() {
	return new weaponEnemy();
}

weapon* weaponsFactory :: getWeaponShip() {
	return new weaponShip();
}

weaponsFactory::~weaponsFactory(){}
