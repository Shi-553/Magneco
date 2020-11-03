#pragma once
#include "myd3d.h"
#include <list>

enum FlyingObjectType
{
	FLYING_OBJECT_NONE,
	FLYING_OBJECT_BLOCK,
	FLYING_OBJECT_ENEMY,
};
typedef struct FlyingObject
{
	D3DXVECTOR2 pos;
	FlyingObjectType type;
	D3DXVECTOR2 dir;
	D3DXVECTOR2 lastPos;

};

std::list<FlyingObject>* GetFlyingObjects();

void InitFlyingObject();
void UninitFlyingObject();
void DrawFlyingObject();
void UpdateFlyingObject();