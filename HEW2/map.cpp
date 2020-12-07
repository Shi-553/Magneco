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

//static Map initGetMap(GetMapHeight(),GetMapWidth())
//{
//	{{MAP_WALL, INTVECTOR2::GetUpperLeftCorner()},    {MAP_WALL, INTVECTOR2::GetUp()},    {MAP_WALL, INTVECTOR2::GetUp()},    {MAP_WALL, INTVECTOR2::GetUp()},  {MAP_WALL, INTVECTOR2::GetUp()},    {MAP_WALL, INTVECTOR2::GetUp()},    {MAP_WALL, INTVECTOR2::GetUp()},   {MAP_WALL, INTVECTOR2::GetUp()},   {MAP_WALL, INTVECTOR2::GetUp()}, {MAP_WALL, INTVECTOR2::GetUpperRightCorner()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                 {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                 {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                        {MAP_GOAL},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                 {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                 {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                       {MAP_ROCK},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                 {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                        {MAP_BLOCK},                   {MAP_BLOCK_NONE},                 {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{          {MAP_WALL, INTVECTOR2::GetRight()},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                 {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                   {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},                  {MAP_BLOCK_NONE},             {MAP_WALL, INTVECTOR2::GetLeft()}},
//	{{MAP_WALL, INTVECTOR2::GetLowreLeftCorner()},  {MAP_WALL, INTVECTOR2::GetDown()}, { MAP_WALL, INTVECTOR2::GetDown()},  {MAP_WALL, INTVECTOR2::GetDown()}, {MAP_WALL, INTVECTOR2::GetDown()}, {MAP_WALL, INTVECTOR2::GetDown()},  {MAP_WALL, INTVECTOR2::GetDown()}, {MAP_WALL, INTVECTOR2::GetDown()}, {MAP_WALL, INTVECTOR2::GetDown()}, {MAP_WALL, INTVECTOR2::GetLowreRightCorner()}},
//
//};

static Map* MapChipList = NULL;
static int textureIds[MAP_MAX];
static int map_textureIds;

static int frame = 0;
static int mapHeight = 0;
static int mapWidth = 0;

void InitMap(void)
{
	map_textureIds = ReserveTextureLoadFile("texture/背景１.png");

	textureIds[MAP_NONE] = ReserveTextureLoadFile("texture/MAP_NONE.png");
	textureIds[MAP_BLOCK_NONE] = ReserveTextureLoadFile("texture/MAP_BLOCK_NONE.png");
	textureIds[MAP_BLOCK] = ReserveTextureLoadFile("texture/block02.png");
	textureIds[MAP_WALL] = ReserveTextureLoadFile("texture/wall.png");
	textureIds[MAP_ROCK] = ReserveTextureLoadFile("texture/brokenblock01.png");
	textureIds[MAP_GOAL] = ReserveTextureLoadFile("texture/warpblock32_64_anime.png");

	frame = 0;

	//MapChipList[][] = {{NULL},{NULL}};


	//MapChipList = new Map[mapHeight * mapWidth]{
	//	{MAP_WALL, INTVECTOR2::GetUpperLeftCorner()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUp()},
	//	{MAP_WALL, INTVECTOR2::GetUpperRightCorner()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_GOAL},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_ROCK},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetRight()},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_BLOCK_NONE},
	//	{MAP_WALL, INTVECTOR2::GetLeft()},

	//	{MAP_WALL, INTVECTOR2::GetLowreLeftCorner()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetDown()},
	//	{MAP_WALL, INTVECTOR2::GetLowreRightCorner()},

	//};

	MapImport("stage/test/test.map");

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
			if (GetMap(j, i).type == MAP_WALL) {
				auto addDir = GetMap(j, i).dir + INTVECTOR2(1, 1);
				auto tPos = D3DXVECTOR2(
					addDir.x * MAP_TEXTURE_WIDTH,               //0  1   2  
					addDir.y * MAP_TEXTURE_HEIGHT               //0 0.3 0.6
				);

				DrawGameSprite(textureIds[GetMap(j, i).type], D3DXVECTOR2(i, j), 100, tPos, D3DXVECTOR2(MAP_TEXTURE_WIDTH, MAP_TEXTURE_HEIGHT));
				continue;
			}

			if (GetMap(j, i).type == MAP_GOAL) {
				auto tPos = D3DXVECTOR2(
					MAP_TEXTURE_WIDTH * (frame / 8 % 8),
					0
				);

				DrawGameSprite(textureIds[GetMap(j, i).type], D3DXVECTOR2(i, j - 1), 100, D3DXVECTOR2(MAP_GOAL_DRAW_SIZE_WIDTH, MAP_GOAL_DRAW_SIZE_HEIGHT), tPos, D3DXVECTOR2(MAP_TEXTURE_WIDTH, MAP_GOAL_TEXTURE_HEIGHT));
			}
			else
			{
				DrawGameSprite(textureIds[GetMap(j, i).type], D3DXVECTOR2(i, j), 100);
			}
		}
	}
}

void MapChange(FlyingObject flyingobject)
{
	auto intPos = flyingobject.trans.GetIntPos();

	if (intPos.x < 0 || intPos.y < 0 || intPos.x >= GetMapWidth() || intPos.y >= GetMapHeight()) {
		return;
	}

	if (flyingobject.type == FLYING_OBJECT_BLOCK) {
		GetMap(intPos.y, intPos.x).type = MAP_BLOCK;
	}
}

bool MapFourDirectionsJudgment(INTVECTOR2 pos)
{
	int x = pos.x;
	int y = pos.y;

	if (GetMap(y + 1, x).type == MAP_BLOCK) {
		return true;
	}
	if (GetMap(y - 1, x).type == MAP_BLOCK) {
		return true;
	}
	if (GetMap(y, x + 1).type == MAP_BLOCK) {
		return true;
	}
	if (GetMap(y, x - 1).type == MAP_BLOCK) {
		return true;
	}

	return false;
}

MapType GetMapType(INTVECTOR2 pos)
{
	int x = pos.x;
	int y = pos.y;

	if (x < 0 || y < 0 || x >= GetMapWidth() || y >= GetMapHeight()) {
		return MAP_NONE;
	}

	return GetMap(y, x).type;
}

bool MapExport(const char* filename) {
	FILE* fp = NULL;

	//	バイナリ書き込みモードでファイルを開く
	fopen_s(&fp, filename, "wb");

	if (fp == NULL) {
		return false;
	}

	//	ファイルへの書き込み処理
	fwrite(&mapHeight, sizeof(int), 1, fp);
	fwrite(&mapWidth, sizeof(int), 1, fp);

	fwrite(MapChipList, sizeof(Map), mapHeight * mapWidth, fp);

	fclose(fp);
	return true;
}

bool MapImport(const char* filename) {
	FILE* fp = NULL;

	//	バイナリ読み込みモードでファイルを開く
	fopen_s(&fp, filename, "rb");

	if (fp == NULL) {
		return false;
	}

	//	ファイルを読み込む処理
	fread(&mapHeight, sizeof(int), 1, fp);
	fread(&mapWidth, sizeof(int), 1, fp);

	if (MapChipList != NULL) {
		delete[] MapChipList;
		MapChipList = NULL;
	}

	MapChipList = new Map[mapHeight * mapWidth];

	fread(MapChipList, sizeof(Map), mapHeight * mapWidth, fp);

	SecureMapLabelList();

	fclose(fp);
	return true;

}

Map& GetMap(int y, int x) {
	return MapChipList[y * mapWidth + x];
}

int GetMapHeight() {
	return mapHeight;
}

int GetMapWidth() {
	return mapWidth;
}
