#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace Constants {

    const int GAME_SPEED = 40;
    const int GAME_REST = 1;

    const int COLOR_DEPTH = 16;
    const int STATUS_HEIGHT = 20;
    const int STATUS_FONT_SIZE = 14;

    const int MAX_ENEMIES = 13;
    const int MAX_ASTEROIDS = 5;

    const int GAME_STATE_RUNNING = 10;
    const int GAME_STATE_LOSS_SHIP = 1; //hit by enemy ship
    const int GAME_STATE_LOSS_ASTEROID = 2; //hit by asteroid
    const int GAME_STATE_LOSS_BULLET = 3; //hit by enemy laser
    const int GAME_STATE_WIN = 4;

    const int UI_ACTION_NONE = 100;
    const int UI_ACTION_EXIT = 2;
    const int UI_ACTION_RESTART = 1;

    const int ACTION_DONOTHING = 0;
    const int ACTION_MOVE_LEFT = 1;
    const int ACTION_MOVE_RIGHT = 2;
    const int ACTION_MOVE_UP = 3;
    const int ACTION_MOVE_DOWN = 4;
    const int ACTION_FIRE = 5;
    const int ACTION_ASTEROID_BOUNCE = 8;
    const int ACTION_ASTEROID_MOVE = 1;
    const int  MAX_ACTION = 20;

    const int OBJECT_ENEMY = 1;
    const int OBJECT_SHIP = 2;
    const int OBJECT_ASTEROID = 3;
    const int OBJECT_BULLET = 4;

    const int SPEED_SPACESHIP = 8;
    const int SPEED_ENEMYSHIP = 5;

    const int YOFFSET_ENEMYSHIP = 10;
    const char *const CREDITS_STR = "TOLIB [abarzev@gmail.com] - Dec, 2008";
    const char *const BMP_PATH_BACKGROUND = "bg.bmp";
    const char *const BMP_PATH_ENEMY = "enemy.bmp";
    const char *const BMP_PATH_ASTEROID = "asteroid.bmp";
    const char *const BMP_PATH_SHIP = "ship.bmp";
}

#endif
