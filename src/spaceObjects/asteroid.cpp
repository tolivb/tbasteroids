#include <allegro.h>
#include <math.h>
#include "asteroid.h"
#include "../objectsManager.h"
#include "../collisions/ebox.h"
#include "../functions.h"
#include "../constants.h"

asteroid::asteroid(int x, int y, int maxX, int maxY) : spaceObject(x, y, maxX, maxY){
    this->obmp  = load_bitmap(Constants::BMP_PATH_ASTEROID, NULL);

    if (!this->obmp) {
        allegro_message("Cannot load asteroid bitmap");
        abort();
    }

    this->speed = 1;
    this->angle = randInt(1, 254);
    this->x1 = x - this->obmp->w/2;
    this->y1 = y - this->obmp->h/2;
    this->x2 = x + this->obmp->w/2;
    this->y2 = y + this->obmp->h/2;

    int tmp = randInt(1, 4);

    switch (tmp) {
        case 1 : 
            this->xd = 1;    
            this->yd = -1;
            break;
        case 2 : 
            this->xd = 1;    
            this->yd = 1;
            break;
        case 3 :
            this->xd = -1;    
            this->yd =  1;
            break;
       case 4 : 
            this->xd = -1;    
            this->yd = -1;
            break;
       default: 
            this->xd = -1;    
            this->yd = -1;
            break;
    }

    this->collide = new ECollide(this->obmp, 3, makecol(255, 0, 255), 40);
    this->recalcCenter();
}


void asteroid :: recalcCenter() {
    this->cx = this->x1 + this->obmp->w/2;
    this->cy = this->y1 + this->obmp->h/2;
}


void asteroid :: draw(BITMAP *bitmap){
    //draw_sprite(bitmap, this->obmp, this->x1, this->y1);
    //256 = 360
    //64  = 90;
    this->angle += 2;
    if (this->angle >= 256) {
        this->angle = 0;
    }

    rotate_sprite(bitmap, this->obmp, this->x1, this->y1, itofix(this->angle));
    //this->collide->display(bitmap, this->cx, this->cy);
}

void asteroid :: doAction(int type=1) {
    switch (type) {
        case  Constants::ACTION_ASTEROID_BOUNCE:
            this->bounce();
            this->recalcCenter();
            break;
         default :
            this->move();
            this->recalcCenter();
    }
}


void asteroid :: bounce(){

    if (!this->getCollisionInProgress()) {
        this->yd = -this->yd;
        this->xd = -this->xd;
    }

    this->x1 += this->speed * this->xd;
    this->y1 += this->speed * this->yd;
    this->x2 += this->speed * this->xd;
    this->y2 += this->speed * this->yd;
}


void asteroid :: move() {
    
    if (this->cx <= 0 && this->xd < 0) {
        this->x1 = this->maxX;
        this->x2 = this->maxX + this->obmp->w;
    }

    if (this->cy <= 0 && this->yd < 0) {
        this->y1 = this->maxY;
        this->y2 = this->maxY + this->obmp->h;
    }

    if (this->cx >= this->maxX && this->xd > 0) {
        this->x1 = 0;
        this->x2 = this->obmp->w;
    }

    if (this->cy >= this->maxY && this->yd > 0) {
        this->y1 = 0;
        this->y2 = this->obmp->h;
    }
    
    this->x1 += this->speed * this->xd;
    this->y1 += this->speed * this->yd;
    this->x2 += this->speed * this->xd;
    this->y2 += this->speed * this->yd;
}

asteroid::~asteroid(){
    destroy_bitmap(this->obmp);
    delete this->collide;
}
