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


static int TextureId = TEXTURE_INVALID_ID;


void InitMap(void)
{
	TextureId = ReserveTextureLoadFile("texture/sample.tga");
}

void UninitMap(void)
{
	ReleaseTexture(&TextureId, 1);
}

void UpdateMap(void)
{

}

void DrawMap(void)
{
	for (int i = 0; i < MAPCHIP_HEIGHT; i++) {
		for (int j = 0; j < MAPCHIP_WIDTH; j++) {
			DrawGameSprite(TextureId, D3DXVECTOR2(i,j), 100);
		}
	}
}

void MapChange(FlyingObject flyingobject)
{
	int x = flyingobject.pos.x;
	int y = flyingobject.pos.y;
    
	if (x < 0 || y < 0 || x >= MAPCHIP_WIDTH || y >= MAPCHIP_HEIGHT){
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