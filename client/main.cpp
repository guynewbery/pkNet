#include "game.h"

int main(void)
{
    BeginGame();

    while (!GameShouldClose())    
    {
        UpdateGame();
        DrawGame();
    }

    EndGame();

    return 0;
}
