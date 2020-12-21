#include "mapEditor.h"
#include "map.h"
#include "texture.h"
#include "sprite.h"
#include "InputLogger.h"
#include "gameSrite.h"
#include "debugPrintf.h"
#include "npc.h"
#include "player.h"


#define CREATE_MAP_TEXTURE_WIDTH 32
#define CREATE_MAP_TEXTURE_HEIGHT 32


#define CREATE_MAP_ZERO_X 100
#define CREATE_MAP_ZERO_Y 150

#define CREATE_MAP_WIDTH 50
#define CREATE_MAP_HEIGHT 50

#define CREATE_MAP_MARGIN 20


//ドラッグでいくつずつ判定するか
#define DRAG_LENGTH 20
//どれくらいでドラッグとみなすか
#define START_DRAG_LENGTH 10


struct ScreenMap {
	Map map;
	D3DXVECTOR2 pos;
};

static int textureIds[MAP_MAX];
static int playerTextureId;
static int npcTextureId;
static int frame = 0;

static INTVECTOR2 createWallDir = { -1,-1 };

//マウスにくっつくMap
static ScreenMap currentMap = {};

//左から作るときはTRUE
static bool isCreate = false;

//ドラッグで持ったらTRUE
static bool isTemp = false;
//ドラッグで持ったMapのGamePos
static D3DXVECTOR2 tempMapPos = {};

static bool isDrag = false;
static D3DXVECTOR2 dragStartPos = {};
static D3DXVECTOR2 lastMousePos = {};

static bool isPlayerEdit = false;
static bool isNPCEdit = false;

ScreenMap CheckCreateMap(D3DXVECTOR2 pos);
D3DXVECTOR2 GetCreateMapPos(MapType type);
void DrawMapScreen(const ScreenMap& map);
ScreenMap CheckMap(D3DXVECTOR2 pos);
bool CheckSquare(const D3DXVECTOR2& target, const D3DXVECTOR2& leftUpPos, const  D3DXVECTOR2& size);

void InitMapEditor() {
	for (int i = 0; i < MAP_MAX; i++)
	{
		textureIds[i] = GetMapTextureId((MapType)i);
	}
	textureIds[MAP_BLOCK_NONE] = ReserveTextureLoadFile("texture/MAP_BLOCK_NONE_ERASE.png");
	playerTextureId= ReserveTextureLoadFile("texture/player_32×32.png");
	npcTextureId= ReserveTextureLoadFile("texture/spr_rose_idle.png");

	frame = 0;
	isDrag = false;
	isCreate = false;
	isTemp = false;
	isPlayerEdit = false;
	isNPCEdit = false;
	
}
void UninitMapEditor() {
	ReleaseTexture(textureIds, MAP_MAX);

}
void DrawMapEditor() {
	for (int i = MAP_BLOCK_NONE; i < MAP_MAX; i++) {
		auto mapType = (MapType)i;
		ScreenMap map;
		map.pos = GetCreateMapPos(mapType);
		map.map.type = mapType;
		map.map.dir = mapType == MAP_WALL ? createWallDir : D3DXVECTOR2(0,0);
		DrawMapScreen(map);
	}

	if (currentMap.map.type != MAP_NONE) {
		ScreenMap map = currentMap;
		map.pos -= D3DXVECTOR2(CREATE_MAP_WIDTH, CREATE_MAP_HEIGHT)/2;
		DrawMapScreen(map);
	}

	auto screenPos = GetCreateMapPos((MapType)-3);

	DrawSprite(playerTextureId, screenPos
		, 10, { 50,50 }, { 0, 0 }, { 32,32 });

	screenPos = GetCreateMapPos((MapType)-2);
	DrawSprite(npcTextureId, screenPos 
		, 10, { 50 ,50 }, { 0, 0 }, { 64,64 });

	if (isPlayerEdit) {
		DrawSprite(playerTextureId, D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY)) - D3DXVECTOR2(32/2,32/2)
			, 10, { 50,50 }, { 0, 0 }, {32,32});
	}
	else {
		DrawPlayer();
	}

	if (isNPCEdit) {
		DrawSprite(npcTextureId, D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY)) - D3DXVECTOR2(50 * 1.76 /2,( 50 * 1.76 /2)+20)
			, 10, { 50* 1.76,50* 1.76 }, { 0, 0 }, {64,64});
	}
	else {
		DrawNPC();
	}
}
void UpdateMapEditor() {
	frame++;
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));
	
	if (!isPlayerEdit && !isNPCEdit) {

		if (TriggerInputLogger(MYVK_LEFT_CLICK)) {
			dragStartPos = mousePos;
		}
		if (PressInputLogger(MYVK_LEFT_CLICK)) {
			if (!isDrag) {
				auto diff = mousePos - dragStartPos;
				float diffLength = D3DXVec2Length(&diff);
				if (diffLength > START_DRAG_LENGTH) {
					isDrag = true;

					if (currentMap.map.type == MAP_NONE) {

						ScreenMap matchedMap = CheckMap(mousePos);
						if (matchedMap.map.type != MAP_NONE && matchedMap.map.type != MAP_BLOCK_NONE) {
							auto gamePos = ScreenToGamePos(matchedMap.pos);
							tempMapPos = gamePos;
							Map* map = GetMap(gamePos);
							if (map != NULL) {
								map->type = MAP_BLOCK_NONE;
								currentMap = matchedMap;
								isCreate = false;
								isTemp = true;
							}
						}
					}
				}
			}
			if (isDrag && !isTemp) {
				auto diff = mousePos - lastMousePos;
				D3DXVECTOR2 diffNomal;
				D3DXVec2Normalize(&diffNomal, &diff);
				float diffLength = D3DXVec2Length(&diff);

				auto pos = lastMousePos;
				for (int i = 0; i < diffLength; i += DRAG_LENGTH)
				{
					pos += diffNomal * DRAG_LENGTH;

					ScreenMap matchedMap = CheckMap(pos);

					//今持ってる
					if (currentMap.map.type != MAP_NONE) {
						if (matchedMap.map.type != MAP_NONE) {
							//チェンジ
							auto gamePos = ScreenToGamePos(matchedMap.pos);
							Map* map = GetMap(gamePos);
							if (map != NULL) {
								*map = currentMap.map;
							}
						}
					}
				}

			}
		}
		if (ReleaseInputLogger(MYVK_LEFT_CLICK)) {

			auto screenPos = GetCreateMapPos((MapType)-3);
			if (CheckSquare(mousePos, screenPos, {50,50})) {
				isPlayerEdit = true;
			}
			screenPos = GetCreateMapPos((MapType)-2);
			if (CheckSquare(mousePos, screenPos, {50,50})) {
				isNPCEdit = true;
			}

			if (!isDrag || isTemp) {
				ScreenMap matchedCreateMap = CheckCreateMap(mousePos);
				ScreenMap matchedMap = CheckMap(mousePos);
				//DebugPrintf("%d %f %f,", matchedCreateMap.map.type, matchedCreateMap.pos.x, matchedCreateMap.pos.y);
				DebugPrintf("%d %f %f,", matchedMap.map.type, matchedMap.pos.x, matchedMap.pos.y);
				DebugPrintf("\n");

				//ドラッグでチェンジ
				if (isTemp) {
					if (matchedMap.map.type == MAP_NONE) {
						Map* map = GetMap(tempMapPos);
						if (map != NULL) {
							*map = currentMap.map;
						}
					}
					else {
						Map* map = GetMap(tempMapPos);
						if (map != NULL) {
							*map = matchedMap.map;
						}

						auto gamePos = ScreenToGamePos(matchedMap.pos);
						map = GetMap(gamePos);
						if (map != NULL) {
							*map = currentMap.map;
						}

					}
					currentMap.map.type = MAP_NONE;
					isCreate = false;
					isTemp = false;
				}
				else {

					//今持ってる
					if (currentMap.map.type != MAP_NONE) {
						if (matchedMap.map.type != MAP_NONE) {
							//置く
							auto gamePos = ScreenToGamePos(matchedMap.pos);
							Map* map = GetMap(gamePos);
							if (map != NULL) {
								*map = currentMap.map;

								if (!isCreate) {
									currentMap.map.type = MAP_NONE;
								}
							}
						}
						else {
							//離す
							currentMap.map.type = MAP_NONE;
						}
					}
					else
						//マップを取る
						if (matchedMap.map.type != MAP_NONE && matchedMap.map.type != MAP_BLOCK_NONE) {
							auto gamePos = ScreenToGamePos(matchedMap.pos);
							Map* map = GetMap(gamePos);
							if (map != NULL) {
								map->type = MAP_BLOCK_NONE;
								currentMap = matchedMap;
								isCreate = false;
								isTemp = false;
							}
						}

					//作るところから取る
					if (matchedCreateMap.map.type != MAP_NONE) {
						currentMap = matchedCreateMap;
						isCreate = true;
						isTemp = false;
					}

				}
			}

			if (isDrag) {
				isDrag = false;
			}
		}

		if (currentMap.map.type != MAP_NONE) {
			currentMap.pos = mousePos;
		}

	}
	else {
		if (ReleaseInputLogger(MYVK_LEFT_CLICK)) {
			ScreenMap matchedMap = CheckMap(mousePos);
			auto scPos = INTVECTOR2( ScreenToGamePos(matchedMap.pos));

			if (matchedMap.map.type != MAP_NONE) {
				if (isPlayerEdit) {
					GetPlayer()->trans.Init(scPos);
					GetPlayer()->trans.pos += D3DXVECTOR2(0.5, 0.5);
					isPlayerEdit = false;
				}
				if (isNPCEdit) {
					GetNpc()->trans.Init(scPos);

					isNPCEdit = false;
				}

			}
		}
	}

	lastMousePos = mousePos;
}

D3DXVECTOR2 GetCreateMapPos(MapType type) {
	D3DXVECTOR2 pos;
	if (type % 2 == 0) {
		pos.x = CREATE_MAP_ZERO_X + CREATE_MAP_WIDTH + CREATE_MAP_MARGIN;
	}
	else {
		pos.x = CREATE_MAP_ZERO_X;
	}

	pos.y = CREATE_MAP_ZERO_Y + (type / 2) * (CREATE_MAP_HEIGHT + CREATE_MAP_MARGIN);

	return pos;
}

ScreenMap CheckCreateMap(D3DXVECTOR2 pos) {
	for (int i = MAP_BLOCK_NONE; i < MAP_MAX; i++) {
		MapType type = (MapType)i;
		auto screenPos = GetCreateMapPos(type);
		D3DXVECTOR2 mapSize = { CREATE_MAP_WIDTH,CREATE_MAP_HEIGHT };
		if (CheckSquare(pos, screenPos, mapSize)) {
			ScreenMap sMap;
			sMap.pos = screenPos;
			sMap.map.type = type;
			sMap.map.dir = type == MAP_WALL ? createWallDir : INTVECTOR2(0, 0);
			return sMap;
		}
	}
	return {};
}

ScreenMap CheckMap(D3DXVECTOR2 pos) {
	auto  gamePos = ScreenToGamePos(pos);
	if (gamePos.x < 0 || gamePos.y < 0 || gamePos.x >= GetMapWidth() || gamePos.y >= GetMapHeight()) {
		return {};
	}
	auto screenPos = GameToScreenPos(gamePos);
	auto m = GetMap(gamePos);
	if (m != NULL) {
		return { *m,screenPos };
	}
	else {
		return { {},screenPos };
	}
}

bool CheckSquare(const D3DXVECTOR2& target, const D3DXVECTOR2& leftUpPos, const  D3DXVECTOR2& size) {
	return (leftUpPos.x < target.x) && (target.x < leftUpPos.x + size.x) &&
		(leftUpPos.y < target.y) && (target.y < leftUpPos.y + size.y);
}

void DrawMapScreen(const ScreenMap& map) {

	D3DXVECTOR2 size = { CREATE_MAP_WIDTH,CREATE_MAP_HEIGHT };

	D3DXVECTOR2 tPos = { 0,0 };
	D3DXVECTOR2 tSize = { CREATE_MAP_TEXTURE_WIDTH,CREATE_MAP_TEXTURE_HEIGHT };

	if (map.map.type == MAP_WALL) {
		auto addDir = map.map.dir + INTVECTOR2(1, 1);
		tPos = D3DXVECTOR2(
			addDir.x * CREATE_MAP_TEXTURE_WIDTH,               //0  1   2  
			addDir.y * CREATE_MAP_TEXTURE_HEIGHT               //0 0.3 0.6
		);
	}
	if (map.map.type == MAP_GOAL) {
		tPos.x = CREATE_MAP_TEXTURE_WIDTH * (frame / 8 % 8);
		tPos.y = CREATE_MAP_TEXTURE_HEIGHT;
	}

	DrawSprite(textureIds[map.map.type], map.pos, 10, size, tPos, tSize);
}