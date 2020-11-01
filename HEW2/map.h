#ifndef MAP_H
#define MAP_H

#include "myd3d.h"

enum MapType
{
	NONE,
	BLOCK_NONE,
	BLOCK,
	WALL,
	ROCK,
	GOAL
};

#define MAPCHIP_WIDTH  (10)
#define MAPCHIP_HEIGHT (10)


void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

void MapChange(FlyingObject flyingobject);

MapType GetMapType(D3DXVECTOR2 pos);

#endif // !MAP_H
