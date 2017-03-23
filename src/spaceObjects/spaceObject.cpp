#include "spaceObject.h"
#include "../functions.h"
#include <iostream>

spaceObject::spaceObject(int x, int y, int maxX, int maxY){
    this->x = x;
    this->y = y;
    this->maxX = maxX;
    this->maxY = maxY;
    this->id = uuid();
    this->collisionObjectId = -1;
    this->collisionInProgress = false;
}

int spaceObject :: getID() {
    return this->id;
}

//returns true only the first time collision occurs
bool spaceObject :: collidesWith(spaceObject* so) {
    bool hasColl = this->collide->Collision(
        so->collide,
        this->cx,
        this->cy,
        so->cx,
        so->cy
    );

    if (hasColl) {
        //ongoing collision
        if (this->id == so->collisionObjectId) {
            this->collisionInProgress = true;
            return false;
        }

        //new collision
        this->collisionObjectId = so->getID();
        so->collisionObjectId = this->getID();
        return true;
    }

    //reset to no collissions
    if (!hasColl && this->collisionObjectId > 0 && this->collisionObjectId == so->getID()) {
        this->collisionObjectId = -1;
        so->collisionObjectId = -1;
        this->collisionInProgress = false;
        return false;
    }

    return false;
}

bool spaceObject :: getCollisionInProgress(){
    return this->collisionInProgress;
}

bool spaceObject::resurect(){
    return true;
}

spaceObject::~spaceObject(){}
