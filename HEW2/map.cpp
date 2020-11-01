#include "map.h"
#include "myd3d.h"
#include "texture.h"


static MapType MapChipList[MAPCHIP_WIDTH][MAPCHIP_HEIGHT]
{
	{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
	{WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL},
	{WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL},
	{WALL, NONE, NONE, NONE, NONE, NONE, GOAL, NONE, NONE, WALL},
	{WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL},
	{WALL, NONE, NONE, NONE, ROCK, NONE, NONE, NONE, NONE, WALL},
	{WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL},
	{WALL, NONE,BLOCK, NONE, NONE, NONE, NONE, NONE, NONE, WALL},
    {WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL},
	{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
};


static int TextureId = TEXTURE_INVALID_ID;


void InitMap(void)
{

}

void UninitMap(void)
{
	//Texture_Release(&TextureId, 1);
}

void UpdateMap(void)
{

}

void DrawMap(void)
{

}
