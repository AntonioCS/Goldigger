
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameSDL.h"
#include "GameState.h"
#include "MapObject.h"
#include "module.h"
#include "player.h"

int main(int argc, char* argv[])
{
    GameState gameState;

    initSDL(&gameState);
    gameLoop(&gameState, 4);
    destroySDL(&gameState);

    return 0;
}
