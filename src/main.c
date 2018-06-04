#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "GameSDL.h"
#include "GameState.h"


int main(int argc, char* argv[])
{
    GameState gameState;

    initSDL(&gameState);

	//gameState.getPlayer().soundDataConfig();

    gameLoop(&gameState, 4);
    destroySDL(&gameState);

    return 0;
}
