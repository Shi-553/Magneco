#include "mapEditor.h"
#include "map.h"
#include "texture.h"
#include "sprite.h"
#include "InputLogger.h"
#include "gameSrite.h"
#include "debugPrintf.h"
#include "npc.h"
#include "player.h"
#include "stageEditor.h"


#define CREATE_MAP_TEXTURE_WIDTH 32
#define CREATE_MAP_TEXTURE_HEIGHT 32


#define CREATE_MAP_ZERO_X 100
#define CREATE_MAP_ZERO_Y 250

#define CREATE_MAP_WIDTH 50
#define CREATE_MAP_HEIGHT 50

#define CREATE_MAP_MARGIN 20


//ドラッグでいくつずつ判定するか
#define DRAG_LENGTH 20
//どれくらいでドラッグとみなすか
#define START_DRAG_LENGTH 10


struct ScreenMap {
	Map* map = NULL;
	D3DXVECTOR2 pos;
};

static int textureIds[MAP_MAX];
static int playerTextureId;
static int npcTextureId;
static int frame = 0;

static INTVECTOR2 createWallDir = { -1,-1 };

//マウスにくっつくMap
static Map currentMap = {};

//左から作るときはTRUE
static bool isCreate = false;

//ドラッグで持ったらTRUE
static bool isTemp = false;
//ドラッグで持ったMap
static ScreenMap tempMap = {};

static ScreenMap rightDragMap = {};

static bool isDrag = false;
static D3DXVECTOR2 dragStartPos = {};
static D3DXVECTOR2 lastMousePos = {};

static bool isPlayerEdit = false;
static bool isNPCEdit = false;

static ScreenMap createMap[MAP_MAX];

ScreenMap CheckCreateMap(D3DXVECTOR2 pos);
D3DXVECTOR2 GetCreateMapPos(MapType type);
void DrawMapScreen(const ScreenMap& map);
ScreenMap CheckMap(D3DXVECTOR2 pos);

void InitMapEditor() {
	for (int i = 0; i < MAP_MAX; i++)
	{
		textureIds[i] = GetMapTextureId((MapType)i);
	}
	textureIds[MAP_BLOCK_NONE] = ReserveTextureLoadFile("texture/MAP_BLOCK_NONE_ERASE.png");
	playerTextureId = ReserveTextureLoadFile("texture/player_32×32.png");
	npcTextureId = ReserveTextureLoadFile("texture/spr_rose_idle.png");

	for (int i = MAP_BLOCK_NONE; i < MAP_MAX; i++) {
		auto mapType = (MapType)i;
		createMap[i].pos = GetCreateMapPos(mapType);

		if (createMap[i].map != NULL) {
			delete createMap[i].map;
			createMap[i].map = NULL;
		}
		createMap[i].map = new Map();

		createMap[i].map->type = mapType;
		createMap[i].map->dir = mapType == MAP_WALL ? createWallDir : D3DXVECTOR2(0, 0);
	}

	frame = 0;
	isDrag = false;
	isCreate = false;
	isTemp = false;
	isPlayerEdit = false;
	isNPCEdit = false;

}
void UninitMapEditor() {
	ReleaseTexture(textureIds, MAP_MAX);

	for (int i = MAP_BLOCK_NONE; i < MAP_MAX; i++) {
		if (createMap[i].map != NULL) {
			delete createMap[i].map;
			createMap[i].map = NULL;
		}
	}
}
void DrawMapEditor() {
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));

	for (int i = MAP_BLOCK_NONE; i < MAP_MAX; i++) {
		DrawMapScreen(createMap[i]);
	}

	if (currentMap.type != MAP_NONE) {
		ScreenMap map;
		map.map = &currentMap;
		map.pos = mousePos - D3DXVECTOR2(CREATE_MAP_WIDTH, CREATE_MAP_HEIGHT) / 2;
		DrawMapScreen(map);
	}

	auto screenPos = GetCreateMapPos((MapType)-3);

	DrawSprite(playerTextureId, screenPos
		, 10, { 50,50 }, { 0, 0 }, { 32,32 });

	screenPos = GetCreateMapPos((MapType)-2);
	DrawSprite(npcTextureId, screenPos
		, 10, { 50 ,50 }, { 0, 0 }, { 64,64 });

	if (isPlayerEdit) {
		DrawSprite(playerTextureId, D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY)) - D3DXVECTOR2(32 / 2, 32 / 2)
			, 10, { 50,50 }, { 0, 0 }, { 32,32 });
	}
	else {
		DrawPlayer();
	}

	if (isNPCEdit) {
		DrawSprite(npcTextureId, D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY)) - D3DXVECTOR2(50 * 1.76 / 2, (50 * 1.76 / 2) + 20)
			, 10, { 50 * 1.76,50 * 1.76 }, { 0, 0 }, { 64,64 });
	}
	else {
		DrawNPC();
	}

	for (int i = 0; i < GetMapHeight(); i++) {
		for (int j = 0; j < GetMapWidth(); j++) {
			auto map = GetMap(INTVECTOR2(j, i));
			if (map!=NULL&&map->type == MAP_CHEST_CLOSED && map->param != 0) {
				FlyingObject item = {};
				item.type = (FlyingObjectType)map->param;
				item.trans.Init(j+0.5, i);
				item.size = {1,1};
				DrawFlyingObject(item);
			}
		}
	}
}
void UpdateMapEditor() {
	if (TriggerInputLogger(MYVK_ARROW_UP)) {
		SetMapHeight(GetMapHeight() + 1);
	}
	if (TriggerInputLogger(MYVK_ARROW_DOWN)) {
		SetMapHeight(GetMapHeight() - 1);
	}
	if (TriggerInputLogger(MYVK_ARROW_LEFT)) {
		SetMapWidth(GetMapWidth() - 1);
	}
	if (TriggerInputLogger(MYVK_ARROW_RIGHT)) {
		SetMapWidth(GetMapWidth() + 1);
	}
	frame++;
}
bool CheckMouseMapEditor() {
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));
	bool ret = false;

	if (!isPlayerEdit && !isNPCEdit) {

		if (TriggerInputLogger(MYVK_RIGHT_CLICK)) {
			dragStartPos = mousePos;
		}
		if (PressInputLogger(MYVK_RIGHT_CLICK)) {
			auto diff = mousePos - dragStartPos;
			if (!isDrag) {
				float diffLength = D3DXVec2Length(&diff);
				if (diffLength > START_DRAG_LENGTH) {
					isDrag = true;
					rightDragMap = CheckMap(dragStartPos);
					if (rightDragMap.map == NULL) {
						rightDragMap = CheckCreateMap(dragStartPos);
					}
				}
			}
			if (isDrag) {
				if (rightDragMap.map != NULL) {
					if (diff.x < -25) {
						diff.x = 1;
					}
					else if (diff.x > 25) {
						diff.x = -1;
					}
					else {
						diff.x = 0;
					}
					if (diff.y < -25) {
						diff.y = 1;
					}
					else if (diff.y > 25) {
						diff.y = -1;
					}
					else {
						diff.y = 0;
					}
					rightDragMap.map->dir = diff;
				}
			}
		}
		if (ReleaseInputLogger(MYVK_RIGHT_CLICK)) {
			isDrag = false;
		}

		if (TriggerInputLogger(MYVK_LEFT_CLICK)) {
			dragStartPos = mousePos;
		}

		//おしぱなし
		if (PressInputLogger(MYVK_LEFT_CLICK)) {
			if (!isDrag) {
				auto diff = mousePos - dragStartPos;
				float diffLength = D3DXVec2Length(&diff);
				if (diffLength > START_DRAG_LENGTH) {
					isDrag = true;

					if (currentMap.type == MAP_NONE) {

						ScreenMap matchedMap = CheckMap(dragStartPos);

						if (matchedMap.map != NULL && matchedMap.map->type != MAP_BLOCK_NONE) {
							tempMap = matchedMap;

							currentMap = *(matchedMap.map);
							matchedMap.map->type = MAP_BLOCK_NONE;
							isCreate = false;
							isTemp = true;

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

					//今持ってるのに変える
					if (currentMap.type != MAP_NONE && matchedMap.map != NULL) {
						*(matchedMap.map) = currentMap;
					}
				}

			}
		}
		if (ReleaseInputLogger(MYVK_LEFT_CLICK)) {

			auto screenPos = GetCreateMapPos((MapType)-3);
			if (CheckSquare(mousePos, screenPos, { 50,50 })) {
				isPlayerEdit = true;
			}
			screenPos = GetCreateMapPos((MapType)-2);
			if (CheckSquare(mousePos, screenPos, { 50,50 })) {
				isNPCEdit = true;
			}

			if (!isDrag || isTemp) {
				ScreenMap matchedCreateMap = CheckCreateMap(mousePos);
				ScreenMap matchedMap = CheckMap(mousePos);
				//DebugPrintf("%d %f %f,", matchedCreateMap.map.type, matchedCreateMap.pos.x, matchedCreateMap.pos.y);
				//DebugPrintf("%d %f %f,", matchedMap.map.type, matchedMap.pos.x, matchedMap.pos.y);
				//DebugPrintf("\n");

				//ドラッグでチェンジ
				if (isTemp) {
					if (matchedMap.map == NULL) {
						*(tempMap.map) = currentMap;
					}
					else {
						*(tempMap.map) = *(matchedMap.map);
						*(matchedMap.map) = currentMap;

					}
					currentMap.type = MAP_NONE;
					isCreate = false;
					isTemp = false;
				}
				else {

					//今持ってる
					if (currentMap.type != MAP_NONE) {
						if (matchedMap.map != NULL) {
							//置く
							*(matchedMap.map) = currentMap;

							if (!isCreate) {
								currentMap.type = MAP_NONE;
							}

						}
						else {
							//離す
							currentMap.type = MAP_NONE;
						}
					}
					else
						//マップを取る
						if (matchedMap.map != NULL && matchedMap.map->type != MAP_BLOCK_NONE) {
							currentMap = (*matchedMap.map);
							matchedMap.map->type = MAP_BLOCK_NONE;
							isCreate = false;
							isTemp = false;

						}

					//作るところから取る
					if (matchedCreateMap.map != NULL) {
						currentMap = *(matchedCreateMap.map);
						isCreate = true;
						isTemp = false;
					}

				}
			}

			if (isDrag) {
				isDrag = false;
			}
		}


	}
	else {
		if (ReleaseInputLogger(MYVK_LEFT_CLICK)) {
			ScreenMap matchedMap = CheckMap(mousePos);
			auto scPos = INTVECTOR2(ScreenToGamePos(matchedMap.pos));

			if (matchedMap.map != NULL) {
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
	return ret;
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
			return createMap[i];
		}
	}
	return {};
}

ScreenMap CheckMap(D3DXVECTOR2 pos) {
	auto  gamePos = ScreenToGamePos(pos);
	auto screenPos = GameToScreenPos(gamePos);
	return { GetMap(gamePos),screenPos };
}


void DrawMapScreen(const ScreenMap& map) {

	D3DXVECTOR2 size = { CREATE_MAP_WIDTH,CREATE_MAP_HEIGHT };

	D3DXVECTOR2 tPos = { 0,0 };
	D3DXVECTOR2 tSize = { CREATE_MAP_TEXTURE_WIDTH,CREATE_MAP_TEXTURE_HEIGHT };

	if (map.map->type == MAP_WALL) {
		auto addDir = map.map->dir + INTVECTOR2(1, 1);
		tPos = D3DXVECTOR2(
			addDir.x * CREATE_MAP_TEXTURE_WIDTH,               //0  1   2  
			addDir.y * CREATE_MAP_TEXTURE_HEIGHT               //0 0.3 0.6
		);
	}
	if (map.map->type == MAP_GOAL) {
		tPos.x = CREATE_MAP_TEXTURE_WIDTH * (frame / 8 % 8);
		tPos.y = CREATE_MAP_TEXTURE_HEIGHT;
	}

	DrawSprite(textureIds[map.map->type], map.pos, 10, size, tPos, tSize);

}