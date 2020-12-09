#pragma once
#include "myd3d.h"
#include <list>
#include "trans.h"


enum FlyingObjectType
{
	FLYING_OBJECT_NONE,
	FLYING_OBJECT_BLOCK,
	FLYING_OBJECT_ENEMY,
	FLYING_OBJECT_UFO,
	FLYING_OBJECT_PLAYER_BLOCK,
	FLYING_OBJECT_PURGE_BLOCK,
};
typedef struct FlyingObject
{
	TRANS trans;
	FlyingObjectType type;
	D3DXVECTOR2 dir;

};

std::list<FlyingObject>* GetFlyingObjects();
void AddFlyingObjects(FlyingObject* flyingObject);

void InitFlyingObject();
void UninitFlyingObject();
void DrawFlyingObject();
void UpdateFlyingObject();


void DrawFlyingObject(FlyingObject flyingObject);


bool UpdateFlyingObject(FlyingObject* flyingObject, float speed);