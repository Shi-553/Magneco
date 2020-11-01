#include "map.h"
#include "texture.h"
#include "flyingObject.h"


static MapType MapChipList[MAPCHIP_HEIGHT][MAPCHIP_WIDTH]
{
	{WALL,       WALL,       WALL,       WALL,       WALL,       WALL,       WALL,       WALL,       WALL, WALL},
	{WALL, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
	{WALL, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
	{WALL, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
	{WALL, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
	{WALL, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE,       ROCK, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
	{WALL, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
	{WALL, BLOCK_NONE,      BLOCK, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
    {WALL, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, BLOCK_NONE, WALL},
	{WALL,       WALL,       WALL,       WALL,       WALL,       WALL,       WALL,       WALL,       WALL, WALL},
};


static int TextureId = TEXTURE_INVALID_ID;


void InitMap(void)
{
	//TextureId = ReserveTextureLoadFile("");
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

void MapChange(FlyingObject flyingobject)
{
	int x = flyingobject.pos.x;
	int y = flyingobject.pos.y;
    
	if (x < 0 || y < 0 || x >= MAPCHIP_WIDTH || y >= MAPCHIP_HEIGHT){
		return;
	}
		
	if (flyingobject.type == FLYING_OBJECT_BLOCK) {
		MapChipList[y][x] = BLOCK;
	}
}

MapType GetMapType(D3DXVECTOR2 pos)
{
	int x = pos.x;
	int y = pos.y;

	if (x < 0 || y < 0 || x >= MAPCHIP_WIDTH || y >= MAPCHIP_HEIGHT) {
		return NONE;
	}

	return MapChipList[y][x];
}