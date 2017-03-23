#include <string>
#include <allegro.h>
#include "enemyShip.h"
#include "../weapons/weaponsFactory.h"
#include "../objectsManager.h"
#include "../collisions/ebox.h"
#include "../functions.h"
#include "../constants.h"


enemyShip::enemyShip(int x, int y, int maxX, int maxY) : spaceObject(x, y, maxX, maxY) {
    this->obmp  = load_bitmap(Constants::BMP_PATH_ENEMY, NULL);

    if (!this->obmp) {
        allegro_message("Cannot load enemy ship sprite: enemy.bmp");
        abort();
    }

    this->x1    = this->obmp->w + x;
    this->y1    = y;
    this->x2    = this->obmp->w + x + this->x1;
    this->y2    = this->y1;
    this->speed = randInt(1, 2);

    this->collide = new ECollide( this->obmp, 3, makecol(255,0,255), 40);
    this->gun     = weaponsFactory :: getWeaponEnemy();

    this->recalcCenter();
}


void enemyShip :: recalcCenter() {
    this->cx = this->x1 + this->obmp->w/2;
    this->cy = this->y1 + this->obmp->h/2;
}

void enemyShip :: doAction(int action) {
    switch (action) {
        case Constants::ACTION_MOVE_LEFT:
            this->moveLeft(5);
            break;
        case Constants::ACTION_MOVE_RIGHT:
            this->moveRight(5);
            break;
        case Constants::ACTION_FIRE:{
            objectsManager* tmp = objectsManager :: getInstance();
            tmp->bulletsEnemy.push_front(this->gun->fire(this->cx, this->cy));
            tmp->sobjects.push_front(tmp->bulletsEnemy.front());
            break;
        }
        default: 
            this->moveDown();
            break;
    }

    this->recalcCenter();
}


void enemyShip :: moveLeft(int pos) {
    int tmpx;
    tmpx = this->x1 - pos;

    if (tmpx <= Constants::YOFFSET_ENEMYSHIP) {
        return;
    }

    this->x1  = tmpx;
    this->x2 -= pos;
}


void enemyShip :: moveRight(int pos) {
    int tmpx;
    tmpx = this->x2 + pos;
    if (tmpx >= (this->maxX - Constants::YOFFSET_ENEMYSHIP)) {
        return;
    }

    this->x2  = tmpx;
    this->x1 += pos;
}


void enemyShip :: moveUp(int pos) {
    int tmpx;
    tmpx = this->y2 - this->speed;

    if (tmpx <= Constants::YOFFSET_ENEMYSHIP) {
        return;
    }

    this->y1 -= this->speed;
    this->y2 -= this->speed;
}


void enemyShip :: moveDown(int pos) {
    int tmpy;
    tmpy = this->y2 + pos;

    if (tmpy >= this->maxY) {
        int rand = randInt(1, 5);
        this->y1 = rand;
        this->y2 = rand;
        this->speed = 1;
    } else {
        if (!(randInt(1, 2500) % 300) && this->speed <= Constants::SPEED_ENEMYSHIP) {
            this->speed++;
        }
        this->y1 += this->speed;
        this->y2 += this->speed;
    }
}


void enemyShip :: draw(BITMAP *bitmap) {
    draw_sprite(bitmap, this->obmp, this->x1, this->y1);
    //this->collide->display(bitmap, this->cx, this->cy);
}


bool enemyShip :: resurect(){
    this->y1 = -3*this->obmp->h;
    this->y2 = this->y1 + this->obmp->h;
    this->recalcCenter();
    return true;
}

enemyShip::~enemyShip(){
    delete this->gun;
    delete this->collide;
    destroy_bitmap(this->obmp);
}
