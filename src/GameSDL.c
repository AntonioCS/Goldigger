#if defined(__APPLE__)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif
#include "GameSDL.h"
#include "GameState.h"
#include "MapObject.h"
#include "module.h"
#include "player.h"

int processEvents(GameState* game)
{

	SDL_Event event;
	int done = 0;
	auto player = game->getPlayer();

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE: {
			if (game->window) {
				SDL_DestroyWindow(game->window);
				game->window = NULL;
				done = 1;
			}
		} break;
		case SDL_KEYDOWN: {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				done = 1;
				break;
			case SDLK_LSHIFT:

				//if (game->man.manDynamites > 0) //
				if (player.hasDynamite()) //
				{
					auto point = player.getLocation();

					game->dynamites[game->nbDynamites].x = point.first;
					game->dynamites[game->nbDynamites].y = point.second;
					game->dynamites[game->nbDynamites].w = 64;
					game->dynamites[game->nbDynamites].h = 64;
					game->dynamites[game->nbDynamites].countdown = 0;
					game->dynamites[game->nbDynamites].used = 0;

					game->nbDynamites++;
					player.decrementDynamiteAmount();
				}
				break;
			}
		} break;
		case SDL_QUIT:
			//quitter le jeu
			done = 1;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (player.m_onLadder && player.m_slowingDown) {
		if (state[SDL_SCANCODE_UP]) {
			player.m_slowingDown = 0;
			player.m_onLedge = 0;
			player.m_climbing = 1;
		}
		else {
			player.m_climbing = 0;
		}
	}
	if (player.m_climbing) {
		//printf("DX: %f, DY: %f\n", game->man.dx, game->man.dy);
		if (!player.m_onLadder || player.m_onLedge) {
			player.m_climbing = 0;
		}
		if (state[SDL_SCANCODE_UP]) {
			player.m_dx = 0;
			player.m_dy -= 0.07;
			if (player.m_dy < -3) {
				player.m_dy = -3;
			}
		}
		else if (state[SDL_SCANCODE_DOWN]) {
			player.m_dx = 0;
			player.m_dy += 0.07;
			if (player.m_dy > 3) {
				player.m_dy = 3;
			}
		}
		else if (state[SDL_SCANCODE_LEFT]) {
			player.m_dx -= 0.07;
			player.m_dy = 0;
			if (player.m_dx < -3) {
				player.m_dx = -3;
			}
		}
		else if (state[SDL_SCANCODE_RIGHT]) {
			player.m_dx += 0.07;
			player.m_dy = 0;
			if (player.m_dx > 3) {
				player.m_dx = 3;
			}
		}
		else {
			player.m_slowingDown = 1;
		}
	}
	else if (!player.m_climbing) {
		if (state[SDL_SCANCODE_LEFT]) {
			player.m_dx -= 0.5;
			if (player.m_dx < -6) {
				player.m_dx = -6;
			}
			player.m_facingLeft = 1;
			player.m_slowingDown = 0; //flag qui me dit que je suis entrain daccelerer
			player.m_walking = 1;
		}
		else if (state[SDL_SCANCODE_RIGHT]) {
			player.m_dx += 0.5;
			if (player.m_dx > 6) {
				player.m_dx = 6;
			}
			player.m_facingLeft = 0;
			player.m_slowingDown = 0;
			player.m_walking = 1;
		}
		else {
			player.m_animFrame = 0;

			player.m_dx *= 0.8f;

			player.m_slowingDown = 1; // dans les autres cas on a appuyé sur rien et le perso slow down
			player.m_walking = 0;

			if (fabsf(player.m_dx) < 0.1f) {
				player.m_dx = 0;
			}
		}

		if (state[SDL_SCANCODE_SPACE] && ((player.m_onLedge) || (player.m_climbing))) {
			player.m_dy = -8;
			player.m_onLedge = 0;
			Mix_PlayChannel(-1, player.m_jumpSound, 0);
		}
	}
	//detects collision on the portal when gold is collected
	auto point = player.getLocation();
	if (player.m_goldAmount == game->nbGolds
		&& (collide2d(point.first, point.second, game->gate.x, game->gate.y, 64, 64, game->gate.w, game->gate.h))) {
		done = 2;
	}

	//man has stepped out of bounds, lets recover him
	if (point.first > 1280 || point.first < 0
		|| point.second > 640 || !player.m_health) {
		done = 1;
	}

	if (done == 1) {
		player.dies();
		//manDies(&game->man);
	}

	return done;
}

void doRenderLedges(GameState* game)
{

	for (int i = 0; i < game->nbLedges; i++) {

		if (game->ledges[i].ledgeType == 1 && game->ledges[i].destroyed == 0) {
			SDL_Rect srcRect_ledges = { 0, 384, game->ledges[i].w, game->ledges[i].h };
			SDL_Rect Rect_ledges = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
			SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_ledges, &Rect_ledges, 0, NULL, SDL_FLIP_NONE);
		}
		else if (game->ledges[i].ledgeType == 2 && game->ledges[i].destroyed == 0) {
			SDL_Rect srcRect_ledges = { 64, 384, game->ledges[i].w, game->ledges[i].h };
			SDL_Rect Rect_ledges = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
			SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_ledges, &Rect_ledges, 0, NULL, SDL_FLIP_NONE);
		}
		else if (game->ledges[i].ledgeType == 7 && game->ledges[i].destroyed == 0) {
			SDL_Rect srcRect_ledges = { 128, 384, game->ledges[i].w, game->ledges[i].h };
			SDL_Rect Rect_ledges = { game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h };
			SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_ledges, &Rect_ledges, 0, NULL, SDL_FLIP_NONE);
		}
	}
}

void doRenderLava_blocks(GameState* game)
{
	int offsetX;
	int offset;

	for (int i = 0; i < game->nbLava_blocks; i++) {
		if (game->lava_blocks[i].lava_anim < 13) {
			offset = 0;
			offsetX = game->lava_blocks[i].lava_anim;
		}
		else if (game->lava_blocks[i].lava_anim < 26) {
			offset = 64;
			offsetX = game->lava_blocks[i].lava_anim - 13;
		}
		else {
			offset = 128;
			offsetX = game->lava_blocks[i].lava_anim - 26;
		}
		//printf(" lava bblocks counter : %d\n",game->lava_blocks[i].lava_anim);
		//printf("offset : %d\n",offset);
		// printf("offsetX : %d\n",offsetX);

		SDL_Rect srcRect_lava = { offsetX * 64, 512 + offset, game->lava_blocks[i].w, game->lava_blocks[i].h };
		SDL_Rect Rect_lava = { game->lava_blocks[i].x, game->lava_blocks[i].y, game->lava_blocks[i].w, game->lava_blocks[i].h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_lava, &Rect_lava, 0, NULL, SDL_FLIP_NONE);
	}
}

void doRenderLadder(GameState* game)
{
	for (int i = 0; i < game->nbLadders; i++) {
		if (game->ladders[i].ladderType == 3) {
			SDL_Rect srcRect_ladder = { 256, 384, game->ladders[i].w, game->ladders[i].h };
			SDL_Rect Rect_ladder = { game->ladders[i].x, game->ladders[i].y, game->ladders[i].w, game->ladders[i].h };
			SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_ladder, &Rect_ladder, 0, NULL, SDL_FLIP_NONE);
			//printf("e.x: %f e.y: %f\n",game->ladders[i].x, game-> ladders[i].y);
		}
		if (game->ladders[i].ladderType == 8) {
			SDL_Rect srcRect_ladder = { 256 + 128, 384, game->ladders[i].w, game->ladders[i].h };
			SDL_Rect Rect_ladder = { game->ladders[i].x, game->ladders[i].y, game->ladders[i].w, game->ladders[i].h };
			SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_ladder, &Rect_ladder, 0, NULL, SDL_FLIP_NONE);
		}
	}
}

void doRenderGold(GameState* game)
{
	for (int i = 0; i < game->nbGolds; i++) {
		if (game->golds[i].display == 1) {
			SDL_Rect srcRect_gold = { 192, 384, game->ledges[i].w, game->ledges[i].h };
			SDL_Rect Rect_gold = { game->golds[i].x, game->golds[i].y, game->golds[i].w, game->golds[i].h };
			SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_gold, &Rect_gold, 0, NULL, SDL_FLIP_NONE);
			//printf("e.x: %f e.y: %f\n",game-> golds[i].x, game-> golds[i].y);
		}
	}
}

void doRenderPortal(GameState* game)
{
	auto player = game->getPlayer();

	if (player.m_goldAmount < game->nbGolds) {
		SDL_Rect srcportalRect = { 0, 704, game->gate.w, game->gate.h };
		SDL_Rect portalRect = { game->gate.x, game->gate.y, game->gate.w, game->gate.h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcportalRect, &portalRect, 0, NULL, SDL_FLIP_NONE);
	}
	else {
		SDL_Rect srcportalRect = { 64, 704, game->gate.w, game->gate.h };
		SDL_Rect portalRect = { game->gate.x, game->gate.y, game->gate.w, game->gate.h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcportalRect, &portalRect, 0, NULL, SDL_FLIP_NONE);
	}
}

void doRenderMan(GameState* game)
{
	auto player = game->getPlayer();
	auto[player_x, player_y] = player.getLocation();
	auto[player_w, player_h] = player.getDimension();

	if (player.m_slowingDown && !player.m_climbing) {
		SDL_Rect srcRect_man = { player.m_animFrame * player_w, 0, player_w, player_h };
		SDL_Rect rect_man = { player_x, player_y, player_w, player_h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_man, &rect_man, 0, NULL, static_cast<SDL_RendererFlip>(player.m_facingLeft));
	}
	else if (!player.m_slowingDown && player.m_onLedge && !player.m_climbing) {
		SDL_Rect srcRect_man = { player.m_animFrame * player_w, 64, player_w, player_h };
		SDL_Rect rect_man = { player_x, player_y, player_w, player_h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_man, &rect_man, 0, NULL, static_cast<SDL_RendererFlip>(player.m_facingLeft));
	}
	else if (!player.m_onLedge && !player.m_climbing) // saut
	{
		SDL_Rect srcRect_man = { player.m_animFrame * player_w, 128, player_w, player_h };

		SDL_Rect rect_man = { player_x, player_y, player_w, player_h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture,
			&srcRect_man, &rect_man, 0, NULL, static_cast<SDL_RendererFlip>(player.m_facingLeft));
	}
	else if (player.m_climbing) {
		SDL_Rect srcRect_man = { player.m_animFrame_onLadder * player_w, 192, player_w, player_h };

		SDL_Rect rect_man = { player_x, player_y, player_w, player_h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture,
			&srcRect_man, &rect_man, 0, NULL, static_cast<SDL_RendererFlip>(player.m_facingLeft));
	}
}

void doRenderDynamite(GameState* game)
{
	// dessine dynamites

	for (int i = 0; i < game->nbDynamites; i++) {
		SDL_Rect srcRect_dyn = { game->dynamites[i].countdown * game->dynamites[i].w, 320, game->dynamites[i].w, game->dynamites[i].h };
		SDL_Rect Rect_dyn = { game->dynamites[i].x, game->dynamites[i].y, game->dynamites[i].w, game->dynamites[i].h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcRect_dyn, &Rect_dyn, 0, NULL, SDL_FLIP_NONE);
	}
}

void doRenderEnemies(GameState* game)
{
	for (int i = 0; i < game->nbEnemies; i++) {
		SDL_Rect srcsnakeRect = {
			game->snake[i].enemy_animFrame * game->snake[i].w,
			384 + 64,
			game->snake[i].w,
			game->snake[i].h
		};
		SDL_Rect snakeRect = { game->snake[i].x, game->snake[i].y, game->snake[i].w, game->snake[i].h };
		SDL_RenderCopyEx(game->renderer, game->sheetTexture, &srcsnakeRect, &snakeRect, 0, NULL, static_cast<SDL_RendererFlip>(game->snake[i].facingLeft));
	}
}

void doRender(GameState* game)
{
	//couleur -> bleu
	SDL_SetRenderDrawColor(game->renderer, 128, 128, 255, 255);

	//clear l'écran (bleu)
	SDL_RenderClear(game->renderer);

	//couleur -> blanc
	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

	SDL_RenderCopy(game->renderer, game->backgroundTexture, NULL, NULL);

	doRenderLedges(game);
	doRenderLava_blocks(game);
	doRenderEnemies(game);
	doRenderLadder(game);
	doRenderGold(game);
	doRenderPortal(game);
	doRenderMan(game);
	doRenderDynamite(game);

	//affiche à l'écran ce qui a été généré
	SDL_RenderPresent(game->renderer);
}

void initSDL(GameState* gameState)
{

	SDL_Window* window = NULL; // Declare une fenetre
	SDL_Renderer* renderer = NULL; // Declare renderer

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); // Initialise SDL2

	//crée une fenêtre avec config suivante
	window = SDL_CreateWindow("Game Window", // window title
		SDL_WINDOWPOS_UNDEFINED, // initial x position
		SDL_WINDOWPOS_UNDEFINED, // initial y position
		1280, // width, en pixels
		640, // height, en pixels
		0 // flags
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	gameState->window = window;
	gameState->renderer = renderer;

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096); //initialize sound
}

void gameLoop(GameState* gameState, int levelMAX)
{
	// fenetre ouverte: entre dans la boucle du jeu ( SDL_PollEvent)

	SetStageNum(gameState, 1);
	loadGame(gameState);
	gameState->musicChannel = Mix_PlayChannel(-1, gameState->bgMusic, -1);
	//initMan(&gameState->man);

	/**
   * 0 = playing current level
   * 1 = lost current level
   * 2 = won current level
   */
	int done;
	auto player = gameState->getPlayer();
	while (gameState->stageNum <= levelMAX && (player.m_lives > 0)) {
		done = 0;
		//boucle evenement
		while (!done) {
			//check evenements
			collisionDetect(gameState);

			done = processEvents(gameState);

			process(gameState);

			//Render display
			doRender(gameState);
		}

		if (done == 2) {
			//destroyLevel(gameState);
			if (gameState->stageNum == levelMAX) {
				//printf("You Won\n");
				break;
			}
			else {
				gameState->stageNum++;
			}
		}
		else if (gameState->stageNum == levelMAX && done == 1) {
			//printf("You Lost\n");
			gameState->stageNum = 1;
		}
		destroyLevel(gameState);
		loadGame(gameState);
	}
}

void destroyLevel(GameState* gameState)
{

	//Shutdown game and unload all memory
	free(gameState->ledges);
	free(gameState->ladders);
	free(gameState->golds);
	free(gameState->dynamites);
	free(gameState->lava_blocks);
	free(gameState->snake);

	//SDL_DestroyTexture(gameState.star);
	SDL_DestroyTexture(gameState->sheetTexture);
	SDL_DestroyTexture(gameState->backgroundTexture);
}

void destroySDL(GameState* gameState)
{

	//Shutdown game and unload all memory
	free(gameState->ledges);
	free(gameState->ladders);
	free(gameState->golds);
	free(gameState->dynamites);
	free(gameState->lava_blocks);
	free(gameState->snake);

	//SDL_DestroyTexture(gameState.star);
	SDL_DestroyTexture(gameState->sheetTexture);
	SDL_DestroyTexture(gameState->backgroundTexture);

	//SDL_DestroyTexture(gameState->blue_portalTexture);
	//SDL_DestroyTexture(gameState->orange_portalTexture);

	//Mix_FreeChunk(gameState->man.dieSound);
	//Mix_FreeChunk(gameState->man.jumpSound);
	//Mix_FreeChunk(gameState->man.ouchSound);
	Mix_FreeChunk(gameState->bombSound);
	Mix_FreeChunk(gameState->goldSound);
	Mix_FreeChunk(gameState->bgMusic);

	// fermeture fenetre
	SDL_DestroyWindow(gameState->window);
	SDL_DestroyRenderer(gameState->renderer);

	// termine prog
	SDL_Quit();
}
