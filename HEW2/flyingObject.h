#pragma once
#include "myd3d.h"

enum FlyingObjectType
{
	FLYING_OBJECT_NONE,
	FLYING_OBJECT_BLOCK,
	FLYING_OBJECT_ENEMY,
};
struct FlyingObject
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 lastPos;
	FlyingObjectType type;
};

std::list<FlyingObject>* GetFlyingObjects();