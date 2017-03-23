#include <string>
#include <allegro.h>
#include "spaceShip.h"
#include "../weapons/weaponsFactory.h"
#include "../objectsManager.h"
#include "../collisions/ebox.h"
#include "../constants.h"

spaceShip::spaceShip(int x, int y, int maxX, int maxY) : spaceObject(x, y, maxX, maxY) {
    this->obmp  = load_bitmap(Constants::BMP_PATH_SHIP, NULL);

    if (!this->obmp) {
        allegro_message("Cannot load spaceship sprite");
        abort();
    }

    this->x1    = x - this->obmp->w/2;
    this->y1    = y - this->obmp->h;
    this->x2    = x + this->obmp->w/2;
    this->y2    = y;
    this->speed = Constants::SPEED_SPACESHIP;

    this->collide = new ECollide(this->obmp, 4, makecol(48,255,0), 40);
    this->gun     = weaponsFactory :: getWeaponShip();

    this->recalcCenter();
}


void spaceShip :: recalcCenter() {
    this->cx = this->x1 + this->obmp->w/2;
    this->cy = this->y1 + this->obmp->h/2;
}


void spaceShip :: doAction(int action) {
    switch (action) {
        case Constants::ACTION_MOVE_LEFT:
            this->moveLeft();
            break;
        case Constants::ACTION_MOVE_RIGHT:
            this->moveRight();
            break;
        case Constants::ACTION_MOVE_UP:
            this->moveUp();
            break;
        case Constants::ACTION_MOVE_DOWN:
            this->moveDown();
            break;
        case Constants::ACTION_FIRE:
            objectsManager* tmp = objectsManager :: getInstance();
            tmp->bulletsShip.push_front(this->gun->fire(this->x1+5, this->cy - this->speed));
            tmp->sobjects.push_front(tmp->bulletsShip.front());
            tmp->bulletsShip.push_front(this->gun->fire(this->x2-5, this->cy - this->speed));
            tmp->sobjects.push_front(tmp->bulletsShip.front());
            break;
    }
    
    this->recalcCenter();
}


void spaceShip :: moveLeft() {
    int tmpx;
    tmpx = this->x1 - this->speed;
    
    if (tmpx <= -Constants::YOFFSET_ENEMYSHIP) {
        return;
    }

    this->x1  = tmpx;
    this->x2 -= this->speed;
}


void spaceShip :: moveRight() {
    int tmpx;
    tmpx = this->x2 + this->speed;
    
    if (tmpx >= this->maxX + Constants::YOFFSET_ENEMYSHIP) {
        return;
    }

    this->x2  = tmpx;
    this->x1 += this->speed;
}


void spaceShip :: moveUp() {
    if (this->y1 <= Constants::YOFFSET_ENEMYSHIP) return;
    this->y1 -= this->speed;
    this->y2 -= this->speed;
}


void spaceShip :: moveDown() {
    int tmpx;
    tmpx = this->y2 + this->speed;
    
    if (tmpx >= (this->maxY + Constants::YOFFSET_ENEMYSHIP * 3)) {
        return;
    }

    this->y1 += this->speed;
    this->y2 += this->speed;
}


void spaceShip :: draw(BITMAP *bitmap) {
    draw_sprite(bitmap, this->obmp, this->x1, this->y1);
    //this->collide->display(bitmap, this->cx, this->cy);
}


spaceShip::~spaceShip(){
    delete this->gun;
    delete this->collide;
    destroy_bitmap(this->obmp);
}
