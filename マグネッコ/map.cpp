#include "map.h"
#include "myd3d.h"
#include "texture.h"
#include "gameSrite.h"
#include "sprite.h"
#include "npc.h"

#define MAP_TEXTURE_WIDTH 32
#define MAP_TEXTURE_HEIGHT 32

#define MAP_GOAL_AND_ITEM_TEXTURE_HEIGHT 64

#define MAP_GOAL_AND_ITEMDRAW_SIZE_WIDTH 1
#define MAP_GOAL_AND_ITEMDRAW_SIZE_HEIGHT 2

#define MAGNETIC_FORCE_TEXTURE_WIDTH (384)
#define MAGNETIC_FORCE_TEXTURE_HEIGHT (64)





void DrawMagnetPower(const D3DXVECTOR2& origin, const D3DXVECTOR2& target);
bool CanAttachedMapType(const INTVECTOR2& pos, MapType type);
bool IsBreakBlock(INTVECTOR2 pos, vector<INTVECTOR2>& v);

static Map* MapChipList = NULL;
static int textureIds[MAP_MAX];
static int map_textureIds;

static int frame = 0;
static int mapHeight = 10;
static int mapWidth = 10;

static int magneticAnimeFrame = 0;


static int putPredictionThinTextureId = TEXTURE_INVALID_ID;
static int magnetPowerWidthTextureId = TEXTURE_INVALID_ID;
static int magnetPowerHeightTextureId = TEXTURE_INVALID_ID;


void InitMap(void)
{
	map_textureIds = ReserveTextureLoadFile("texture/background/背景１.png");

	textureIds[MAP_NONE] = ReserveTextureLoadFile("texture/block/MAP_NONE.png");
	textureIds[MAP_BLOCK_NONE] = ReserveTextureLoadFile("texture/block/MAP_BLOCK_NONE.png");
	textureIds[MAP_BLOCK] = ReserveTextureLoadFile("texture/block/road_block.png");
	textureIds[MAP_CHAECKPOINT_ON] = ReserveTextureLoadFile("texture/block/link_block.png");
	textureIds[MAP_CHAECKPOINT_OFF] = ReserveTextureLoadFile("texture/block/point_block.png");
	textureIds[MAP_WALL] = ReserveTextureLoadFile("texture/block/wall.png");
	textureIds[MAP_ROCK] = ReserveTextureLoadFile("texture/block/road_block_broken.png");
	textureIds[MAP_GOAL] = ReserveTextureLoadFile("texture/block/warpblock32_64_anime.png");
	textureIds[MAP_CHEST_CLOSED] = ReserveTextureLoadFile("texture/block/itembox_anime.png");
	textureIds[MAP_CHEST_OPENED] = ReserveTextureLoadFile("texture/block/itembox_block.png");
	textureIds[MAP_BLOCK_REMOVE] = ReserveTextureLoadFile("texture/block/road_block.png");
	putPredictionThinTextureId = ReserveTextureLoadFile("texture/player/putPredictionThin.png");
	magnetPowerWidthTextureId = ReserveTextureLoadFile("texture/effect/connect_electricity.png");
	magnetPowerHeightTextureId = ReserveTextureLoadFile("texture/effect/connect_electricity_rotate.png");
	//magnetPowerHeightTextureId = ReserveTextureLoadFile("texture/block/magnetPowerHeight.png");

	frame = 0;
	mapHeight = 10;
	mapWidth = 10;
	magneticAnimeFrame = 0;

	if (MapChipList != NULL) {
		delete[] MapChipList;
		MapChipList = NULL;
	}
#ifdef _DEBUG

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
		{MAP_CHEST_CLOSED,{},FLYING_OBJECT_CHECKPOINT_OFF},
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
		{MAP_CHAECKPOINT_ON},
		{MAP_BLOCK_NONE},
		{MAP_BLOCK_NONE},
		{MAP_CHEST_CLOSED,{},FLYING_OBJECT_CHECKPOINT_OFF},
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

#endif // _DEBUG
}

void UninitMap(void)
{
	ReleaseTexture(textureIds, MAP_MAX);
	ReleaseTexture(map_textureIds);
	ReleaseTexture(putPredictionThinTextureId);
	ReleaseTexture(magnetPowerWidthTextureId);
	ReleaseTexture(magnetPowerHeightTextureId);

	if (MapChipList != NULL) {
		delete[] MapChipList;
		MapChipList = NULL;
	}

}

void UpdateMap(void)
{
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			Map* map = GetMap(INTVECTOR2(j, i));
			if (map == NULL) {
				continue;
			}
			if (map->type == MAP_BLOCK_REMOVE) {
				map->param--;
				if (map->param < 0) {
					map->type = MAP_BLOCK_NONE;
				}
			}
		}
	}
	frame++;
	magneticAnimeFrame++;
}


void DrawMap(void)
{
	DrawSprite(map_textureIds, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			auto mapPos = INTVECTOR2(j, i);
			Map* map = GetMap(mapPos);
			if (map == NULL) {
				continue;
			}
			if (map->type == MAP_BLOCK_REMOVE) {
				auto size = map->param / 100.0f;
				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(j + 0.5 - size / 2, i + 0.5 - size / 2), 100, D3DXVECTOR2(size*0.9, size*0.9));
				continue;
			}
			if (map->type == MAP_BLOCK_NONE && MapFourDirectionsJudgment(mapPos)) {
				EndMapFourDirectionsJudgment();
				DrawGameSprite(putPredictionThinTextureId, D3DXVECTOR2(j, i), 100);

				continue;
			}
			EndMapFourDirectionsJudgment();
			if (map->type == MAP_WALL) {
				auto addDir = map->dir + INTVECTOR2(1, 1);
				auto tPos = D3DXVECTOR2(
					addDir.x * MAP_TEXTURE_WIDTH,               //0  1   2  
					addDir.y * MAP_TEXTURE_HEIGHT               //0 0.3 0.6
				);

				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(j, i), 100, tPos, D3DXVECTOR2(MAP_TEXTURE_WIDTH, MAP_TEXTURE_HEIGHT));
				continue;
			}

			if (map->type == MAP_GOAL || map->type == MAP_CHEST_CLOSED) {
				auto tPos = D3DXVECTOR2(
					MAP_TEXTURE_WIDTH * (frame / 8 % 8),
					0
				);

				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(j + 0.05f, i - 0.8f), 100, D3DXVECTOR2(MAP_GOAL_AND_ITEMDRAW_SIZE_WIDTH*0.9, MAP_GOAL_AND_ITEMDRAW_SIZE_HEIGHT*0.9), tPos, D3DXVECTOR2(MAP_TEXTURE_WIDTH, MAP_GOAL_AND_ITEM_TEXTURE_HEIGHT));

				continue;
			}
			if (map->type == MAP_CHEST_OPENED ||map->type ==  MAP_CHAECKPOINT_ON ||map->type ==  MAP_CHAECKPOINT_OFF) {
				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(j + 0.05f, i+ 0.1f), 100, D3DXVECTOR2(0.9, 0.9));
				continue;
			}

			if (map->type != MAP_BLOCK) {
				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(j, i), 100);

			}

			if (map->type == MAP_BLOCK) {
				DrawGameSprite(textureIds[map->type], D3DXVECTOR2(j + 0.05f, i + 0.1f), 100, D3DXVECTOR2(0.9, 0.9));

				continue;
			}

		}

	}
	
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			auto mapPos = INTVECTOR2(j, i);
			Map* map = GetMap(mapPos);
			if (map == NULL) {
				continue;
			}
			if (map->type == MAP_BLOCK) {

				DrawMagnetPower(D3DXVECTOR2(j, i), D3DXVECTOR2(j + 1, i));
				DrawMagnetPower(D3DXVECTOR2(j, i), D3DXVECTOR2(j - 1, i));
				DrawMagnetPower(D3DXVECTOR2(j, i), D3DXVECTOR2(j, i + 1));
				DrawMagnetPower(D3DXVECTOR2(j, i), D3DXVECTOR2(j, i - 1));
				continue;
			}
		}
	}

}

void DrawMagnetPower(const D3DXVECTOR2& origin, const D3DXVECTOR2& target) {
	auto isW = origin.x != target.x;

	auto type = GetMapType(target);
	if (GetMapType(origin)== MAP_BLOCK &&type == MAP_BLOCK&&(origin.x<target.x||origin.y<target.y)) {
		return;
	}
	if (CanAttachedMapType(target, type)) {
		EndMapFourDirectionsJudgment();
		auto pos = (origin + target) / 2;
		if (isW) {
			pos.y -= 0.05f;

			auto tPos = D3DXVECTOR2(
			0,
			MAGNETIC_FORCE_TEXTURE_HEIGHT * (magneticAnimeFrame / 12 % 2)
			);

			pos.x += 0.26f;
			DrawGameSprite(magnetPowerWidthTextureId, pos, 100, D3DXVECTOR2(0.5f, 1), tPos, D3DXVECTOR2(MAGNETIC_FORCE_TEXTURE_HEIGHT, MAGNETIC_FORCE_TEXTURE_WIDTH / 6));

			//DrawGameSprite(magnetPowerWidthTextureId, pos, 100);
		}
		else {
			//pos.y -= 0.1f;
			//pos.x -= 0.05f;

			auto tPos = D3DXVECTOR2(
			MAGNETIC_FORCE_TEXTURE_HEIGHT * (magneticAnimeFrame / 12 % 6),
			0
			);

			pos.y += 0.1f;
			DrawGameSprite(magnetPowerHeightTextureId, pos, 100, D3DXVECTOR2(1, 0.8f), tPos, D3DXVECTOR2(MAGNETIC_FORCE_TEXTURE_HEIGHT, MAGNETIC_FORCE_TEXTURE_WIDTH / 6));
		}
	}
}

void MapChange(FlyingObject flyingobject)
{
	auto& intPos = flyingobject.trans.GetIntPos();


	Map* map = GetMap(intPos);

	if (map != NULL && flyingobject.type == FLYING_OBJECT_PLAYER_BLOCK) {
		map->type = MAP_BLOCK;
	}
	if (map != NULL && flyingobject.type == FLYING_OBJECT_CHECKPOINT_OFF) {
		map->type = MAP_CHAECKPOINT_OFF;
	}

}

//無限ループしないように
vector<INTVECTOR2> v;
bool MapFourDirectionsJudgment(const INTVECTOR2& pos)
{
	if (std::find(v.begin(), v.end(), pos) != v.end()) {
		return false;
	}
	v.push_back(pos);

	int x = pos.x;
	int y = pos.y;

	auto to = INTVECTOR2(x, y + 1);
	if (std::find(v.begin(), v.end(), to) != v.end()) {
		return false;
	}

	Map* map = GetMap(to);

	if (map != NULL && CanAttachedMapType(to, map->type)) {
		return true;
	}
	to = INTVECTOR2(x, y - 1);
	if (std::find(v.begin(), v.end(), to) != v.end()) {
		return false;
	}
	map = GetMap(to);
	if (map != NULL && CanAttachedMapType(to, map->type)) {
		return true;
	}

	to = INTVECTOR2(x + 1, y);
	if (std::find(v.begin(), v.end(), to) != v.end()) {
		return false;
	}
	map = GetMap(to);
	if (map != NULL && CanAttachedMapType(to, map->type)) {
		return true;
	}

	to = INTVECTOR2(x - 1, y);
	if (std::find(v.begin(), v.end(), to) != v.end()) {
		return false;
	}
	map = GetMap(to);
	if (map != NULL && CanAttachedMapType(to, map->type)) {
		return true;
	}

	return false;
}

void EndMapFourDirectionsJudgment() {
	v.clear();
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


	return true;

}

Map* GetMap(INTVECTOR2 pos) {

	if (pos.x < 0 || pos.y < 0 || pos.x >= mapWidth || pos.y >= mapHeight) {
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

void SetMapHeight(int h) {
	if (h <= 0 || h == mapHeight) {
		return;
	}

	Map* temp = new Map[mapWidth * h];

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < mapWidth; j++) {
			if (i < mapHeight) {
				temp[i * mapWidth + j] = MapChipList[i * mapWidth + j];
			}
			else {
				temp[i * mapWidth + j].dir = INTVECTOR2(0, 0);
				temp[i * mapWidth + j].param = 0;
				temp[i * mapWidth + j].type = MAP_BLOCK_NONE;

			}
		}
	}
	if (MapChipList != NULL) {
		delete[] MapChipList;
		MapChipList = NULL;
	}
	mapHeight = h;

	MapChipList = temp;
	//Grid_Finalize();
	//Grid_Initialize(GAME_SPRITE_WHIDTH, mapHeight<mapWidth?mapWidth:mapHeight, D3DCOLOR_RGBA(0, 197, 0, 255));
}

void SetMapWidth(int w) {
	if (w <= 0 || w == mapWidth) {
		return;
	}
	Map* temp = new Map[mapHeight * w];

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < w; j++) {
			if (j < mapWidth) {
				temp[i * w + j] = MapChipList[i * mapWidth + j];
			}
			else {
				temp[i * w + j].dir = INTVECTOR2(0, 0);
				temp[i * w + j].param = 0;
				temp[i * w + j].type = MAP_BLOCK_NONE;

			}
		}
	}
	if (MapChipList != NULL) {
		delete[] MapChipList;
		MapChipList = NULL;
	}
	mapWidth = w;

	MapChipList = temp;
	//Grid_Finalize();
	//Grid_Initialize(GAME_SPRITE_WHIDTH, mapHeight < mapWidth ? mapWidth : mapHeight, D3DCOLOR_RGBA(0, 197, 0, 255));

}

bool CanGoNPCMapType(MapType type) {
	switch (type)
	{
	case MAP_BLOCK:
	case MAP_GOAL:
	case MAP_CHEST_CLOSED:
	case MAP_CHEST_OPENED:
	case MAP_CHAECKPOINT_ON:
	case MAP_CHAECKPOINT_OFF:
		return true;
		break;
	default:
		return false;
		break;
	}
}
bool CanAttachedMapType(const INTVECTOR2& pos, MapType type) {
	switch (type)
	{
	case MAP_CHEST_OPENED:
	case MAP_CHEST_CLOSED:
	case MAP_GOAL:
		return MapFourDirectionsJudgment(pos);

	case MAP_BLOCK:
	case MAP_CHAECKPOINT_ON:
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
	f.hp = 1;
	f.size = { 1,1 };
	AddFlyingObjects(&f);
}


int GetMapTextureId(MapType type) {
	if (type < MAP_NONE || MAP_MAX <= type) {
		return TEXTURE_INVALID_ID;
	}
	return textureIds[(int)type];
}

bool BreakNotConnectBlock(INTVECTOR2 pos) {
	vector<INTVECTOR2> v;

	if (IsBreakBlock(pos, v)) {
		return false;
	}
	for (auto itrV = v.begin(); itrV != v.end(); itrV++) {
		Map* map = GetMap(*itrV);
		if (map != NULL && map->type == MAP_BLOCK) {
			map->type = MAP_BLOCK_REMOVE;
			map->param = 100;
		}
	}
	return true;
}

bool IsBreakBlock(INTVECTOR2 pos, vector<INTVECTOR2>& v) {
	//既にみてたらスルー
	if (std::find(v.begin(), v.end(), pos) != v.end()) {
		return false;
	}
	Map* m = GetMap(pos);
	//範囲外
	if (m == NULL) {
		return false;
	}
	//チェックポイント
	if (m->type == MAP_CHAECKPOINT_ON) {
		return true;
	}
	//くっつけられない
	if (!CanAttachedMapType(pos, m->type)) {
		EndMapFourDirectionsJudgment();
		return false;
	}
	EndMapFourDirectionsJudgment();
	v.push_back(pos);

	//そこからみて4方向チェック
	if (IsBreakBlock(pos + INTVECTOR2(0, 1), v) ||
		IsBreakBlock(pos + INTVECTOR2(0, -1), v) ||
		IsBreakBlock(pos + INTVECTOR2(1, 0), v) ||
		IsBreakBlock(pos + INTVECTOR2(-1, 0), v)) {
		return true;
	}

	return false;
}
