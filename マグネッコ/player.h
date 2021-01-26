#ifndef _PLAYER_H
#define _PLAYER_H
#include <list>
#include <vector>
#include "flyingObject.h"
#include "trans.h"
#pragma once

#define DEFAULT_PUT_REQUIRED_FRAME (int)(1.5 * 60)

#define DEFAULT_PLAYER_STAN_FRAME (1 * 60)
#define DEFAULT_PLAYER_INVICIBLE_FRAME (2 * 60)

struct Player {
	TRANS trans;
	std::list <FlyingObject> flyingObjectList;
	std::list <FlyingObject> purgeFlyingObjectList;
	D3DXVECTOR2 dir;
	float baseSpeed;
	float addSpeed;
	int frame;
	int blockMax;
	bool checkCheckpoint;
	bool isPut;
	bool isMove;
	int putFrame;
	int putRequiredFrame;
	int stanTime;
	int invicibleTime;
	int nekopunchTime;
	D3DXVECTOR2 size;
	std::vector<FlyingObject> putFlyingObjectList;
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

Player* GetPlayer();

void PutBeacon();
void PurgePlayerFlyingObject();

bool PlayerExport(FILE* fp);
bool PlayerImport(FILE* fp);

void MakePut();
void PutCansel();
bool GetBlock(FlyingObject& itr, D3DXVECTOR2& attachPos);

bool IsPlayerInvicible();

bool DamagePlayer();
bool DamagePlayerFlyingObject(int uid);

bool RemoteBlockToFreeFlyingObject();

void GetItem(FlyingObjectType type);
#endif // !_PLAYER_H
