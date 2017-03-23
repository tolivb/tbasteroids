#include <allegro.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include "gameManager.h"
#include "objectsManager.h"
#include "constants.h"

volatile int ticks = 0;
void ticker() { ticks++; } END_OF_FUNCTION(ticker)

gameManager* gameManager :: gmanager = 0;

gameManager* gameManager::getInstance() {
    if (!gmanager) {
        gmanager = new gameManager();
    }

    return gmanager;
}


gameManager :: gameManager() {
    int ret = allegro_init();

    if (ret != 0) {
        std::cout << "allegro_init failed:" << allegro_error << "\n";
        exit(1);
    }

    install_keyboard();
    install_timer();
    
    this->gameScore = 0;
    this->gameState = Constants::GAME_STATE_RUNNING;
    this->bgImgPath = Constants::BMP_PATH_BACKGROUND;
    this->initDisplay();
    this->initObjectManager();
}


void gameManager :: initDisplay(){
    set_color_depth(Constants::COLOR_DEPTH);

    PALETTE palette;
    this->background = load_bitmap(this->bgImgPath, palette);

    if (!this->background) {
        allegro_message("Unable to load arena background");
        abort();
    }

    this->screenWidth  = this->background->w;
    this->screenHeight = this->background->h;

    this->arenaWidth   = this->background->w;
    this->arenaHeight  = this->screenHeight - Constants::STATUS_HEIGHT;

    this->arenaX       = 0;
    this->arenaY       = 0;

    this->statusWidth  = this->background->w;
    this->statusHeight = Constants::STATUS_HEIGHT;
    this->statusX      = 0;
    this->statusY      = this->screenHeight - Constants::STATUS_HEIGHT;

    this->arena     = create_bitmap (this->screenWidth, this->screenHeight);
    this->statusBar = create_bitmap (this->statusWidth, this->statusHeight);
    this->vbuffer   = create_bitmap(this->screenWidth, this->screenHeight);

    if (!this->arena || !this->statusBar || !this->vbuffer) {
        allegro_message("Unable to create inmemory bitmaps...");
        abort();
    }

    int gfx_mode = set_gfx_mode (
            GFX_AUTODETECT_WINDOWED,
            this->screenWidth,
            this->screenHeight,
            0,
            0
    );

    if ( gfx_mode < 0) {
        allegro_message("Unable to set graphics mode - %s.\n", allegro_error);
        abort();
    }
}


void gameManager :: initObjectManager(){
    this->omanager = objectsManager :: getInstance();
    this->omanager->setParams(
        this->arena,
        Constants::MAX_ASTEROIDS,
        Constants::MAX_ENEMIES
    );
}


void gameManager :: updateScore(int points=1) {
    this->gameScore += points;
}

    
void gameManager :: refreshStatusBar(){
    clear(this->statusBar);

    int offsetY = 5;
    int offsetX = 5;

    textprintf_ex (
            this->statusBar, 
            font, /*comes from allegro*/
            offsetX,
            offsetY,
            makecol(0,255,94),
            -1,
            "SCORE: %d ",
            this->gameScore
    );

    char buf[200];

//    sprintf(buf, "ENEMIES: %d", (int)this->omanager->enemies.size());
//    textout_ex (
//        this->statusBar,
//        font,
//        buf ,
//        100 + offsetX,
//        offsetY,
//        makecol(255,255,0),
//        -1
//    );

    sprintf(buf, "ASTEROIDS: %d", (int)this->omanager->asteroids.size());
    textout_ex (
        this->statusBar,
        font,
        buf ,
        100 + offsetX,
        offsetY,
        makecol(255,255,0),
        -1
    );

    sprintf(buf, "ACTIVE ENEMY BULLETS: %d", (int)this->omanager->bulletsEnemy.size());
    textout_ex (
        this->statusBar,
        font,
        buf ,
        215 + offsetX,
        offsetY,
        makecol(238,0,0),
        -1
    );

    sprintf(buf, "ACTIVE SHIP BULLETS: %d", (int)this->omanager->bulletsShip.size());
    textout_ex (
        this->statusBar,
        font,
        buf ,
        425 + offsetX,
        offsetY,
        makecol(0,171,255),
        -1
    );


    sprintf(buf, Constants::CREDITS_STR);
    textout_right_ex (
        this->statusBar,
        font,
        buf ,
        this->screenWidth - offsetX,
        offsetY,
        makecol(255,153,255),
        -1
    );
}


void gameManager :: startGame(){
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);

    install_int_ex(ticker, BPS_TO_TIMER(Constants::GAME_SPEED));

    this->action = Constants::ACTION_DONOTHING;
    while(true) {
        //game loop
        while (!key[KEY_ESC] && this->gameState == Constants::GAME_STATE_RUNNING) {
            //game sync
            while(ticks == 0) {
                rest(Constants::GAME_REST);
            }

            while(ticks > 0) {
                int tmpTicks = ticks;

                //game logic and input
                this->inputScan();
                this->omanager->runObjectsLogic(this->action);
                this->action = Constants::ACTION_DONOTHING;
                ticks--;

                if(tmpTicks <= ticks) break;
            }

            //rendering
            this->render();
        }

        //game ui
        if(this->exitGame()) {
            break;
        }

        ticks = 0;
    }
}


void gameManager :: inputScan(){
    //reading from key makes the move smoother
    if (key[KEY_LEFT])  this->action = Constants::ACTION_MOVE_LEFT;
    if (key[KEY_RIGHT]) this->action = Constants::ACTION_MOVE_RIGHT;
    if (key[KEY_UP])    this->action = Constants::ACTION_MOVE_UP;
    if (key[KEY_DOWN])  this->action = Constants::ACTION_MOVE_DOWN;

    //use readkey+keypressed to fire once at a time
    if (keypressed()) {
        int tmp = readkey();
        if ((tmp>>8) == KEY_SPACE) {
            this->action = Constants::ACTION_FIRE;
        }
    }
}


void gameManager :: render(){
    //prepare background
    clear_bitmap(this->arena);

    blit(
        this->background,
        this->arena,
        0,0,0,0,
        this->arenaWidth,
        this->arenaHeight
    );

    //prepare space objects
    this->omanager->renderObjects();

    //prepare status bar
    this->refreshStatusBar();

    //add status combine the status bar and the arena
    clear_bitmap(this->vbuffer);
    blit(
        this->statusBar,
        this->vbuffer,
        0,
        0,
        this->statusX,
        this->statusY,
        this->statusWidth,
        this->statusHeight
    );

    blit(
        this->arena,
        this->vbuffer,
        this->arenaX,
        this->arenaY,
        this->arenaX,
        this->arenaY,
        this->arenaWidth,
        this->arenaHeight
    );

    //show everything on the screen
    blit(this->vbuffer, screen, 0,0,0,0, this->screenWidth, this->screenHeight);
}

bool gameManager :: exitGame() {
    int userAction = Constants::UI_ACTION_NONE;
    //handle game UI
    switch  (this->gameState) {
        case Constants::GAME_STATE_LOSS_ASTEROID :
            userAction = alert(
                "LOSS - Next time use the force and don't crash on asteroids...",
                " ",
                "ESC to quit...",
                "&Restart", //btn1
                NULL, //btn2
                'r', 0
            );
            break;
        case Constants::GAME_STATE_LOSS_SHIP :
        case Constants::GAME_STATE_LOSS_BULLET :
            userAction = alert(
                "LOSS - The Dark side defeated you...",
                " ",
                "ESC to quit...",
                "&Restart", //btn1
                NULL, //btn2
                'r', 0
            );
            break;
        case Constants::GAME_STATE_WIN :
            userAction = alert(
                "WIN - Obi-Wan has taught you well. The Galaxy is safe again...",
                " ",
                " ",
                "&Restart", //btn1
                NULL, //btn2
                'r', 0
            );
            break;
        case Constants::GAME_STATE_RUNNING :
            userAction = alert(
                "To QUIT press ESC...",
                "",
                "",
                "&Continue", //btn1
                NULL, //btn2
                'c', 0
            );
            break;
    }

    if (userAction == Constants::UI_ACTION_EXIT) {
        return true;
    }

    if (this->gameState != Constants::GAME_STATE_RUNNING &&
            userAction == Constants::UI_ACTION_RESTART){
        this->omanager->restart();
    }

    this->gameState = Constants::GAME_STATE_RUNNING;

    return false;
}


void gameManager :: setGameState(int type) {
    this->gameState = type;
}


void gameManager :: setWin() {
    this->setGameState(Constants::GAME_STATE_WIN);
}


void gameManager :: setLossBullet() {
    this->setGameState(Constants::GAME_STATE_LOSS_BULLET);
}


void gameManager :: setLossAsteroid() {
    this->setGameState(Constants::GAME_STATE_LOSS_ASTEROID);
}


void gameManager :: setLossShip() {
    this->setGameState(Constants::GAME_STATE_LOSS_SHIP);
}


gameManager :: ~gameManager(){
    destroy_bitmap (this->arena);
    destroy_bitmap (this->background);
    destroy_bitmap (this->statusBar);
    destroy_bitmap(this->vbuffer);
    delete this->omanager;
    allegro_exit();    
}
