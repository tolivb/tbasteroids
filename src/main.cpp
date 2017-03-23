#include "gameManager.h"

int main(void) {
    gameManager* gm = gameManager :: getInstance();
    gm->startGame();
    delete gm;
    return 0;
}
END_OF_MAIN()
