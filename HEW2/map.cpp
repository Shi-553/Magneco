#include "map.h"
#include "myd3d.h"
#include "texture.h"
#include "gameSrite.h"



static MapType MapChipList[MAPCHIP_HEIGHT][MAPCHIP_WIDTH]
{
	{MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE,       MAP_ROCK, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE,      MAP_BLOCK, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_BLOCK_NONE, MAP_WALL},
	{MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL,       MAP_WALL, MAP_WALL},

};


static int textureIds[MAP_MAX];


void InitMap(void)
{

	textureIds[MAP_NONE] = ReserveTextureLoadFile("MAP_NONE.tga");
	textureIds[MAP_BLOCK_NONE] = ReserveTextureLoadFile("MAP_BLOCK_NONE.tga");
	textureIds[MAP_BLOCK] = ReserveTextureLoadFile("MAP_BLOCK.tga");
	textureIds[MAP_WALL] = ReserveTextureLoadFile("MAP_WALL.tga");
	textureIds[MAP_ROCK] = ReserveTextureLoadFile("MAP_ROCK.tga");
	textureIds[MAP_GOAL] = ReserveTextureLoadFile("MAP_GOAL.tga");
}
void UninitMap(void)
{
	for (int i = 0; i < MAP_MAX; i++)
	{
		ReleaseTexture(&textureIds[i], 1);
	}
}

void UpdateMap(void)
{

}

void DrawMap(void)
{
	for (int i = 0; i < MAPCHIP_HEIGHT; i++) {
		for (int j = 0; j < MAPCHIP_WIDTH; j++) {
			DrawGameSprite(textureIds[MapChipList[i][j]], D3DXVECTOR2(i, j), 100);
		}
	}
}

void MapChange(FlyingObject flyingobject)
{
	int x = flyingobject.pos.x;
	int y = flyingobject.pos.y;

	if (x < 0 || y < 0 || x >= MAPCHIP_WIDTH || y >= MAPCHIP_HEIGHT) {
		return;
	}

	if (flyingobject.type == FLYING_OBJECT_BLOCK) {
		MapChipList[y][x] = MAP_BLOCK;
	}
}

MapType GetMapType(D3DXVECTOR2 pos)
{
	int x = pos.x;
	int y = pos.y;

	if (x < 0 || y < 0 || x >= MAPCHIP_WIDTH || y >= MAPCHIP_HEIGHT) {
		return MAP_NONE;
	}

	return MapChipList[y][x];
}