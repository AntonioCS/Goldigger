#include "Player.hpp"

void Player::soundDataConfig() noexcept
{
	m_dieSound = Mix_LoadWAV("files/audio/die.wav");
	Mix_VolumeChunk(m_dieSound, 40);
	m_ouchSound = Mix_LoadWAV("files/audio/ouch.wav");
	Mix_VolumeChunk(m_ouchSound, 40);
	m_jumpSound = Mix_LoadWAV("files/audio/jump.wav");
}

Player::Player()
{
	soundDataConfig();
}


Player::~Player()
{
	Mix_FreeChunk(m_dieSound);
	Mix_FreeChunk(m_ouchSound);
	Mix_FreeChunk(m_jumpSound);
}

void Player::initManLevel() noexcept
{

	m_dx = 0;
	m_dy = 0;

	setLocation(240, 540);

	//man->x = 240;
	//man->y = 540;
	//man->w = 48;
	//man->h = 64;
	m_onLedge = 0;
	m_onLadder = 0;
	m_animFrame = 0;
	m_animFrame_onLadder = 0;
	m_facingLeft = 0;
	m_slowingDown = 0;
	m_walking = 0;
	m_jumping = 0;
	m_climbing = 0;
	m_goldAmount = 0;
	m_dynamiteAmount = 10;
	m_health = 3;

}

void Player::decrementDynamiteAmount(int byAmount) noexcept
{
	if (m_dynamiteAmount - byAmount < 0) {
		m_dynamiteAmount = 0;
	}
	else {
		m_dynamiteAmount -= byAmount;
	}
}

int Player::getDynamiteAmount() const noexcept
{
	return m_dynamiteAmount;
}

bool Player::hasDynamite() const noexcept
{
	return (getDynamiteAmount() > 0);
}

void Player::dies() noexcept
{
	Mix_PlayChannel(-1, m_dieSound, 0);
	if (m_lives) {
		m_lives--;
	}
}
