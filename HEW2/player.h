#ifndef _PLAYER_H
#define _PLAYER_H
#include <list>
#include "flyingObject.h"


typedef struct Player {
	D3DXVECTOR2 position;
	D3DXVECTOR2 lastPosition;
	std::list < FlyingObject > flyingObjectList;
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

#endif // !_PLAYER_H
