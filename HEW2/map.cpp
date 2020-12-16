#include "map.h"
#include "myd3d.h"
#include "texture.h"
#include "gameSrite.h"
#include "sprite.h"
#include "npc.h"

#define MAP_TEXTURE_WIDTH 32
#define MAP_TEXTURE_HEIGHT 32

#define MAP_GOAL_TEXTURE_HEIGHT 64

#define MAP_GOAL_DRAW_SIZE_WIDTH 1
#define MAP_GOAL_DRAW_SIZE_HEIGHT 2

bool CanAttachedMapType(MapType type);

static Map* MapChipList = NULL;
static int textureIds[MAP_MAX];
static int map_textureIds;

static int frame = 0;
static int mapHeight = 10;
static int mapWidth = 10;

void InitMap(void)
{
	map_textureIds = ReserveTextureLoadFile("texture/背景１.png");

	textureIds[MAP_NONE] = ReserveTextureLoadFile("texture/MAP_NONE.png");
	textureIds[MAP_BLOCK_NONE] = ReserveTextureLoadFile("texture/MAP_BLOCK_NONE.png");
	textureIds[MAP_BLOCK] = ReserveTextureLoadFile("texture/block02.png");
	textureIds[MAP_UNBREAKABLE_BLOCK] = ReserveTextureLoadFile("texture/unbreakableBlock.png");
	textureIds[MAP_WALL] = ReserveTextureLoadFile("texture/wall.png");
	textureIds[MAP_ROCK] = ReserveTextureLoadFile("texture/brokenblock01.png");
	textureIds[MAP_GOAL] = ReserveTextureLoadFile("texture/warpblock32_64_anime.png");
	textureIds[MAP_CHEST_CLOSED] = ReserveTextureLoadFile("texture/chestClose.png");
	textureIds[MAP_CHEST_OPENED] = ReserveTextureLoadFile("texture/chestOpen.png");

	frame = 0;


	MapChipList = new Map[mapHeight * mapWidth]{
		{MAP_WALL, INTVECTOR2::GetUpperLeftCorner()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUp()},
		{MAP_WALL, INTVECTOR2::GetUpperRightCorner()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_CHEST_CLOSED,{},FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_GOAL},
		{MAP_BLOCK_NONE},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},	
		{MAP_CHEST_CLOSED,{},FLYING_OBJECT_ITEM_ADD_SPEED},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},	
		{MAP_CHEST_CLOSED,{},FLYING_OBJECT_ITEM_CHAGE_BLOCK_UNBREAKABLE},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_ROCK},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_UNBREAKABLE_BLOCK},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetRight()},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_WALL, INTVECTOR2::GetLeft()},

		{MAP_WALL, INTVECTOR2::GetLowreLeftCorner()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetDown()},
		{MAP_WALL, INTVECTOR2::GetLowreRightCorner()},

	};

	SecureMapLabelList();
}

void UninitMap(void)
{
	ReleaseTexture(textureIds, MAP_MAX);
	ReleaseTexture(map_textureIds);

	if (MapChipList != NULL) {
		delete[] MapChipList;
		MapChipList = NULL;
	}

}

void UpdateMap(void)
{
	frame++;
}


void DrawMap(void)
{
	DrawSprite(map_textureIds, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });

	for (int i = 0; i < GetMapHeight(); i++) {
		for (int j = 0; j < GetMapWidth(); j++) {
			Map* map = GetMap(INTVECTOR2(i, j));
			if (map == NULL) {
				continue;
			}
			if (map->type == MAP_WALL) {
				auto addDir = map->dir + INTVECTOR2(1, 1);
				auto tPos = D3DXVECTOR2(
					addDir.x * MAP_TEXTURE_WIDTH,               //0  1   2  
					addDir.y * MAP_TEXTURE_HEIGHT               //0 0.3 0.6
				);

				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(i, j), 100, tPos, D3DXVECTOR2(MAP_TEXTURE_WIDTH, MAP_TEXTURE_HEIGHT));
				continue;
			}

			if (map->type == MAP_GOAL) {
				auto tPos = D3DXVECTOR2(
					MAP_TEXTURE_WIDTH * (frame / 8 % 8),
					0
				);

				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(i, j - 1), 100, D3DXVECTOR2(MAP_GOAL_DRAW_SIZE_WIDTH, MAP_GOAL_DRAW_SIZE_HEIGHT), tPos, D3DXVECTOR2(MAP_TEXTURE_WIDTH, MAP_GOAL_TEXTURE_HEIGHT));
			}
			else
			{
				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(i, j), 100);
			}
		}
	}
}

void MapChange(FlyingObject flyingobject)
{
	auto& intPos = flyingobject.trans.GetIntPos();

	if (intPos.x < 0 || intPos.y < 0 || intPos.x >= GetMapWidth() || intPos.y >= GetMapHeight()) {
		return;
	}

	Map* map = GetMap(intPos);

	if (map != NULL && flyingobject.type == FLYING_OBJECT_PLAYER_BLOCK) {
		map->type = MAP_BLOCK;
	}
}

bool MapFourDirectionsJudgment(INTVECTOR2 pos)
{
	int x = pos.x;
	int y = pos.y;

	Map* map = GetMap(INTVECTOR2(x, y + 1));

	if (map != NULL && CanAttachedMapType(map->type)) {
		return true;
	}

	map = GetMap(INTVECTOR2(x, y - 1));
	if (map != NULL && CanAttachedMapType(map->type)) {
		return true;
	}

	map = GetMap(INTVECTOR2(x + 1, y));
	if (map != NULL && CanAttachedMapType(map->type)) {
		return true;
	}

	map = GetMap(INTVECTOR2(x - 1, y));
	if (map != NULL && CanAttachedMapType(map->type)) {
		return true;
	}

	return false;
}

MapType GetMapType(INTVECTOR2 pos)
{

	Map* map = GetMap(pos);
	if (map == NULL) {
		return MAP_NONE;
	}

	return map->type;
}

bool MapExport(FILE* fp) {

	//	ファイルへの書き込み処理
	fwrite(&mapHeight, sizeof(int), 1, fp);
	fwrite(&mapWidth, sizeof(int), 1, fp);

	fwrite(MapChipList, sizeof(Map), mapHeight * mapWidth, fp);

	return true;
}

bool MapImport(FILE* fp) {

	if (MapChipList != NULL) {
		delete[] MapChipList;
		MapChipList = NULL;
	}

	//	ファイルを読み込む処理
	fread(&mapHeight, sizeof(int), 1, fp);
	fread(&mapWidth, sizeof(int), 1, fp);


	MapChipList = new Map[mapHeight * mapWidth];


	fread(MapChipList, sizeof(Map), mapHeight * mapWidth, fp);

	SecureMapLabelList();

	return true;

}

Map* GetMap(INTVECTOR2 pos) {

	if (pos.x < 0 || pos.y < 0 || pos.x >= GetMapWidth() || pos.y >= GetMapHeight()) {
		return NULL;
	}

	return &MapChipList[pos.y * mapWidth + pos.x];
}

int GetMapHeight() {
	return mapHeight;
}

int GetMapWidth() {
	return mapWidth;
}

bool CanGoNPCMapType(MapType type) {
	switch (type)
	{
	case MAP_BLOCK:
	case MAP_GOAL:
	case MAP_CHEST_CLOSED:
	case MAP_CHEST_OPENED:
	case MAP_UNBREAKABLE_BLOCK:
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool CanAttachedMapType(MapType type) {
	switch (type)
	{
	case MAP_BLOCK:
	case MAP_GOAL:
	case MAP_CHEST_OPENED:
	case MAP_UNBREAKABLE_BLOCK:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void OpenChest(INTVECTOR2 pos) {
	Map* map = GetMap(pos);
	if (map == NULL || map->type != MAP_CHEST_CLOSED) {
		return;
	}
	map->type = MAP_CHEST_OPENED;
	auto p = pos.ToD3DXVECTOR2();
	p.x += 0.5;
	p.y -= 0.5;
	FlyingObject f = { TRANS(p),(FlyingObjectType)map->param,{0,0} };
	AddFlyingObjects(&f);
}
