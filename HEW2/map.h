#ifndef MAP_H
#define MAP_H

enum Map_Object
{
	NONE,
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

#endif // !MAP_H
