#include <allegro.h>
#include <string>
#include "objectsManager.h"

#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

class gameManager{
    
public:
    static gameManager* getInstance();
    void updateScore(int);
    void refreshStatusBar();
    void startGame();
    void setGameState(int);
    void setWin();
    void setLossBullet();
    void setLossShip();
    void setLossAsteroid();
    bool exitGame();
    ~gameManager();
    
private:
    gameManager();

    void inputScan();
    void initDisplay();
    void initObjectManager();
    void render();

    int action;
    int screenWidth, screenHeight;
    int arenaWidth, arenaHeight, arenaX, arenaY;
    int statusWidth, statusHeight, statusX, statusY;
    int gameState, gameScore;

    BITMAP *arena, *background, *statusBar, *vbuffer;
    objectsManager *omanager;

    const char* bgImgPath;
    char* spaceShipImgPath;
    char* enemyShipImgPath;
    char* asteroidImgPath;

    static gameManager* gmanager;
};

#endif /*GAMEMANAGER_H_*/
