#ifndef _PLAYER_H
#define _PLAYER_H
#include <list>
#include "flyingObject.h"
#include "trans.h"
#pragma once

#define BLOCK_PUT_FRAME (3 * 60)

struct Player {
	TRANS trans;
	std::list <FlyingObject> flyingObjectList;
	std::list <FlyingObject> purgeFlyingObjectList;
	D3DXVECTOR2 dir;
	float speed;
	int frame;
	int blockMax;
	int blockPutFrame;
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
void MovePlayer(D3DXVECTOR2 dir);

void BlockDecision();

Player* GetPlayer();

void PutBeacon();
void PurgePlayerFlyingObject();

bool PlayerExport(FILE* fp);
bool PlayerImport(FILE* fp);

#endif // !_PLAYER_H
