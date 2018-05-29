/**
 @file MapObject.h
 @brief Fichier entête du module MapObject.c.
 @author Alexandre HASSLER Idir DJEMAOUNE Eloi POYET
 @date 23 MAI 2016
 */

#pragma once

struct Ledge
{
  float x, y, w, h;
  int ledgeType;
  int destroyed;
};

struct Ladder
{
  float x, y, w, h;
  int ladderType;
};

struct Gold
{
  float x, y, w, h;
  int display;
};

struct Portal
{
  float x, y, w, h;
  int open;
};

struct Dynamite
{
  float x, y, w, h;
  int countdown;
  int used;
};

struct Lava
{
  float x, y, w, h;
  int lava_anim;
  //int lavaType;
};

struct Enemy
{
  float x, y, w, h;
  int enemy_animFrame;
  int facingLeft;
};


/**
 @fn  int Ledge_getX (Ledge * l);
 @brief game_getLedgeX:Accesseur à une structure legde
 @param [in, out] l pointeur sur ledge
 @return int
 */

int Ledge_getX (Ledge * l);