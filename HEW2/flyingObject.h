#pragma once
#include "myd3d.h"
#include <list>
#include "trans.h"

#define ADD_UFO_POS (D3DXVECTOR2(0.5f, -1.0f))

enum FlyingObjectType
{
	FLYING_OBJECT_NONE,
	FLYING_OBJECT_BLOCK,
	FLYING_OBJECT_ENEMY,
	FLYING_OBJECT_ENEMY_BREAK_BLOCK,
	FLYING_OBJECT_UFO,
	FLYING_OBJECT_ENEMY_SECOND,
	FLYING_OBJECT_PLAYER_BLOCK,
	FLYING_OBJECT_PURGE_BLOCK,
	FLYING_OBJECT_ITEM_ADD_SPEED,
	FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE,
	FLYING_OBJECT_CHECKPOINT_OFF,
	FLYING_OBJECT_MAX	
};
typedef struct FlyingObject
{
	TRANS trans;
	FlyingObjectType type;
	D3DXVECTOR2 dir;
	int id;
	bool isAnime;
	int hp;
	INTVECTOR2 size;
	float speed;
};

std::list<FlyingObject>* GetFlyingObjects();
void AddFlyingObjects(FlyingObject* flyingObject);

void InitFlyingObject();
void UninitFlyingObject();
void DrawFlyingObject();
void UpdateFlyingObject();


void DrawFlyingObject(FlyingObject flyingObject);


bool UpdateFlyingObject(FlyingObject* flyingObject, float speed);

bool IsFlyingObjectItem(FlyingObjectType type);
bool UpdateFlyingObject(FlyingObject* flyingObject, float speed);

void BackFlyingObject(int frame);

void DestroyUFO();

int GetFlyingObjectTextureId(FlyingObjectType type);