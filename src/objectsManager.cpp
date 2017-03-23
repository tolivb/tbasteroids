#include <iostream>
#include <ctime>
#include <string>
#include <allegro.h>
#include "objectsManager.h"
#include "spaceObjects/spaceObjectsFactory.h"
#include "spaceObjects/spaceObject.h"
#include "spaceObjects/bullet.h"
#include "collisions/ebox.h"
#include "functions.h"
#include "collisions/ebox.h"
#include "gameManager.h"
#include "constants.h"

objectsManager* objectsManager::omanager = 0;
objectsManager::objectsManager() {}


void objectsManager :: setParams(BITMAP* arena, int maxAsteroids, int maxEnemies) {
    this->battlefield  = arena;
    this->maxEnemies   = maxEnemies;
    this->maxAsteroids = maxAsteroids;
    this->init();
}


objectsManager* objectsManager::getInstance() {
    if (omanager == 0) {
        omanager = new objectsManager();
    }
    return omanager;
}


void objectsManager::runObjectsLogic(int kbdaction) {
    gameManager *gmanager = gameManager :: getInstance();
    /**
     * SpaceShip
     */
    if (!this->runShipLogic(kbdaction )) {
        return;
    }
    /**
     * Asteroids
     */
    if (!this->runAsteroidsLogic()) {
        return;
    }

    /**
     * Enemies
     */
    if (!this->runEnemiesLogic()) {
        return;
    }

    if ((int)this->enemies.size() == 0) {
        //Win
        gmanager->setWin();
    }

    return;
}

bool objectsManager::runAsteroidsLogic(){
    gameManager *gmanager = gameManager :: getInstance();
    std :: list<spaceObject*> :: iterator it;
    std :: list<spaceObject*> :: iterator it1;
    bool hasCollision = false;
    /**
     * Asteroids
     */
    for (it = this->asteroids.begin(); it != this->asteroids.end(); it++) {
        for (it1 = this->asteroids.begin(); it1 != this->asteroids.end(); it1++) {
            if (it == it1) continue;

            if ((*it)->collidesWith((*it1))) {
                hasCollision = true;
                break;
            }
        }

        /**
        * Asteroid hits another asteroid
        */
        if (hasCollision) {
            hasCollision = false;
            (*it)->doAction(Constants::ACTION_ASTEROID_BOUNCE);
            (*it1)->doAction(Constants::ACTION_ASTEROID_BOUNCE);
            continue;
        }

        /**
         * Asteroid hits spaceship
         */
        if ((*it)->collidesWith(this->spaceShip)) {
            gmanager->setLossAsteroid();
            return false;
        }

        /**
         * Absorb bullets
         */
        for (it1 = this->bulletsShip.begin();it1 != this->bulletsShip.end(); it1++) {
            if ((*it)->collidesWith((*it1))) {
                delete *it1;
                it1 = this->bulletsShip.erase(it1);
                continue;
            }
        }

        for (it1 = this->bulletsEnemy.begin();it1 != this->bulletsEnemy.end(); it1++) {
            if ((*it)->collidesWith((*it1))) {
                delete *it1;
                it1 = this->bulletsEnemy.erase(it1);
                continue;
            }
        }

        (*it)->doAction(Constants::ACTION_ASTEROID_MOVE);
    }

    return true;
}

bool objectsManager::runEnemiesLogic(){
    gameManager *gmanager = gameManager :: getInstance();
    std :: list<spaceObject*> :: iterator it;
    std :: list<spaceObject*> :: iterator it1;
    /**
     * Enemies
     */
    for (it = this->enemies.begin(); it != this->enemies.end(); it++) {
        /**
         * Enemy hits spaceship
         */
         if ((*it)->collidesWith(this->spaceShip)) {
             gmanager->setLossShip();
             return false;
         }

         for (it1 = this->bulletsShip.begin();it1 != this->bulletsShip.end(); it1++) {
             /**
              * Enemy is shoot down
              */
             if ((*it)->collidesWith((*it1))) {
                 //delete *it;
                 //this->enemies.erase(it1);
                 (*it)->resurect();

                 delete *it1;
                 it1 = this->bulletsShip.erase(it1);
                 gmanager->updateScore(1);
                 break;
             }
         }

        //random move (left,right,down) - AI:)
        (*it)->doAction(randInt(1, 100));
    }

    for (it = this->bulletsEnemy.begin();it != this->bulletsEnemy.end(); it++) {
        if ((*it)->collidesWith(this->spaceShip)) {
            gmanager->setLossShip();
            return false;
        }

        /**
         * Bullet is out of range
         */
        if ((*it)->cy >= this->battlefield->h || (*it)->cy <= 1) {
            delete *it;
            it = this->bulletsEnemy.erase(it);
            continue;
        }

        (*it)->doAction(0);
    }

    return false;
}

bool objectsManager::runShipLogic(int kbdaction) {
    std :: list<spaceObject*> :: iterator it;
    std :: list<spaceObject*> :: iterator it1;

    /**
     * SpaceShip
     */
    if (kbdaction) {
        this->spaceShip->doAction(kbdaction);
    }

    for (it = this->bulletsShip.begin();it != this->bulletsShip.end(); it++) {
        for (it1 = this->bulletsEnemy.begin();it1 != this->bulletsEnemy.end(); it1++) {
            if ((*it)->collidesWith((*it1))) {
                delete *it;
                it = this->bulletsShip.erase(it);

                delete *it1;
                it1 = this->bulletsEnemy.erase(it1);
                break;
            }
        }
        /**
         * Bullet is out of range
         */
        if ((*it)->cy >= this->battlefield->h || (*it)->cy <= 1) {
            delete *it;
            it = this->bulletsShip.erase(it);
            continue;
        }

        (*it)->doAction(0);
    }

    return true;
}

void objectsManager::renderObjects() {
    std :: list<spaceObject*> :: iterator it;

    this->spaceShip->draw(this->battlefield);

    for (it = this->bulletsEnemy.begin(); it != this->bulletsEnemy.end(); it++) {
        (*it)->draw(this->battlefield);
    }

    for (it = this->bulletsShip.begin(); it != this->bulletsShip.end(); it++) {
        (*it)->draw(this->battlefield);
    }

    for (it = this->asteroids.begin(); it != this->asteroids.end(); it++) {
        (*it)->draw(this->battlefield);
    }

    for (it = this->enemies.begin(); it != this->enemies.end(); it++) {
        (*it)->draw(this->battlefield);
    }
}


void objectsManager::init() {
    srand(time(NULL));
    /**
     * create spaceship
     */
    spaceObject* tmp = spaceObjectsFactory :: getShip(
        this->battlefield->w/2,
        this->battlefield->h,
        this->battlefield->w,
        this->battlefield->h
    );

    this->sobjects.push_front(tmp);
    this->spaceShip  = tmp;
    
    /**
     * create enemies
     */
    for (int i = 0; i < this->maxEnemies; i++) {
        tmp = this->generateEnemy();
        this->enemies.push_front(tmp);
        this->sobjects.push_front(tmp);
    }
    
    /**
     * create asteroids
     */
    for (int i = 0; i < this->maxAsteroids; i++) {
        tmp = this->generateAsteroid();
        this->asteroids.push_front(tmp);
        this->sobjects.push_front(tmp);
    }
}


spaceObject* objectsManager :: generateEnemy() {
    int genX=0, genY=0;
    std :: list<spaceObject*> :: iterator it;
    it = this->enemies.begin();

    if (it!= this->enemies.end()) {
        genX = (*it)->cx;
        genY = randInt(0, (*it)->cy);
    }

    return spaceObjectsFactory :: getEnemy(
        genX,
        genY,
        this->battlefield->w,
        this->battlefield->h
    );
}


spaceObject* objectsManager :: generateAsteroid(){
    int genX=0, genY=0;
    std :: list<spaceObject*> :: iterator it;
    genX = randInt(50, this->battlefield->w);
    genY = randInt(100, this->battlefield->h - 300);
    return spaceObjectsFactory :: getAsteroid(
        genX,
        genY,
        this->battlefield->w,
        this->battlefield->h
    );
}


void objectsManager :: restart() {
    std :: list<spaceObject*> :: iterator it;

    for (it = this->bulletsEnemy.begin(); it != this->bulletsEnemy.end(); it++) {
        delete *it;
    }

    for (it = this->bulletsShip.begin(); it != this->bulletsShip.end(); it++) {
        delete *it;
    }

    for (it = this->asteroids.begin(); it != this->asteroids.end(); it++) {
        delete *it;
    }

    for (it = this->enemies.begin(); it != this->enemies.end(); it++) {
        delete *it;
    }

    delete this->spaceShip;

    this->sobjects.clear();
    this->bulletsEnemy.clear();
    this->bulletsShip.clear();
    this->enemies.clear();
    this->asteroids.clear();

    this->init();
}


objectsManager :: ~objectsManager() {
    this->sobjects.clear();
}
