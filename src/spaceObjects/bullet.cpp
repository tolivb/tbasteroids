#include <allegro.h>
#include "bullet.h"
#include "../collisions/ebox.h"

bullet::bullet(int x, int y, int d, int color): spaceObject(x, y, 0, 0)
{
    this->obmp      = create_bitmap(4, 8);
    this->x1        = x;
    this->y1        = y;
    this->x2        = x + this->obmp->w;
    this->y2        = this->y1 + this->obmp->h;
    this->direction = d;
    this->speed     = 5;

    clear_to_color(this->obmp, makecol(0, 0, 0));

    if (color == 1) {
        color = makecol(238, 0, 0); //red
    } else {
        color = makecol(0, 171, 255); //blue
    }

    rectfill(this->obmp, 0, 0, this->obmp->w, this->obmp->h, color);
    this->collide = new ECollide( this->obmp, 1, makecol(0, 0, 0), 1 );
    this->recalcCenter();
}


void bullet :: recalcCenter() {
    this->cx  = this->x1 + this->obmp->w/2;
    this->cy  = this->y2 - (this->obmp->h/2)*this->direction;
}


void bullet :: doAction(int type){
    this->y1 = this->y1 + (this->speed * this->direction);
    this->y2 = this->y2 + (this->speed * this->direction);
    this->recalcCenter();
}


void bullet :: draw(BITMAP *bitmap){
    int y = this->y1;

    if (this->direction < 0) {
        y = this->y2;
    }

    blit(this->obmp, bitmap, 0, 0, this->x1, y, this->obmp->w, this->obmp->h);
    //this->collide->display(bitmap,this->cx,this->cy);
}

bullet::~bullet(){
    delete this->collide;
    destroy_bitmap(this->obmp);
}
