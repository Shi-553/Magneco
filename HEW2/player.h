#ifndef _PLAYER_H
#define _PLAYER_H


typedef struct Player_tag {
	D3DXVECTOR2 position;
	std::list < FlyingObject > flyingObjectList;
}Player;

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
