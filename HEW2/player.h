#ifndef _PLAYER_H
#define _PLAYER_H
#include <list>
#include "flyingObject.h"
#include "trans.h"
#pragma once

typedef struct Player {
	TRANS trans;
	std::list < FlyingObject > flyingObjectList;
	bool isMove;
};

void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();

void RotateLeftPlayer();
void RotateRightPlayer();

void MoveUpPlayer();
void MoveDownPlayer();
void MoveLeftPlayer();
void MoveRightPlayer();

void BlockDecision();

Player* GetPlayer();

void PutBeacon();

#endif // !_PLAYER_H
