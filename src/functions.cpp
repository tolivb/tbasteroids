#include <cstdlib>
#include "functions.h"

int randInt(int from, int to) {
    return rand() % (to - from) + from;
}


int uuid(){
    static int id = 0;
    return id++;
}
