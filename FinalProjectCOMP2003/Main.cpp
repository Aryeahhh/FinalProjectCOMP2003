#include "Game.h"
#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(0)));
    LotteryGame game;
    game.play();
    return 0;
}
