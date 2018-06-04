#pragma once

#include <SDL2/SDL_mixer.h>

#include "Entity.h"

class Player : public Acs::Entity
{
public:
	float m_dx;
	float m_dy;

	int m_onLedge;
	int m_onLadder;

	int m_animFrame;
	int m_animFrame_onLadder;

	int m_facingLeft;
	int m_slowingDown;

	int m_walking;
	int m_jumping;
	int m_climbing;
	int m_falling;

	int m_goldAmount = 0;
	int m_dynamiteAmount = 0;

	unsigned short int m_health;
	unsigned short int m_lives = 3;

	Mix_Chunk *m_ouchSound = nullptr;
	Mix_Chunk *m_dieSound = nullptr;
	Mix_Chunk *m_jumpSound = nullptr;

	void soundDataConfig() noexcept;
public:
	Player();
	~Player();
	
	void initManLevel() noexcept;
	//void manHurt();
	//void manDies();

	void decrementDynamiteAmount(int byAmount = 1) noexcept;
	int getDynamiteAmount() const noexcept;
	bool hasDynamite() const noexcept;

	void dies() noexcept;
};

