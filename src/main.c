#include <stdio.h>

#if defined(__APPLE__)
#include "SDL.h"
#include "SDL_image.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
//#include "SDL_mixer.h"
//#include "SDL2/SDL_ttf.h"
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
