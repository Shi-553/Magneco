#include "mapEditor.h"
#include "map.h"
#include "texture.h"
#include "sprite.h"
#include "InputLogger.h"
#include "gameSrite.h"
#include "debugPrintf.h"


#define CREATE_MAP_TEXTURE_WIDTH 32
#define CREATE_MAP_TEXTURE_HEIGHT 32

#define CREATE_MAP_ZERO_X 100
#define CREATE_MAP_ZERO_Y 150

#define CREATE_MAP_WIDTH 50
#define CREATE_MAP_HEIGHT 50

#define CREATE_MAP_MARGIN 20

struct ScreenMap {
	Map map;
	D3DXVECTOR2 pos;
};

static int textureIds[MAP_MAX];
static int frame = 0;

static INTVECTOR2 createWallDir = { -1,-1 };

static ScreenMap currentMap = {};

ScreenMap CheckCreateMap(D3DXVECTOR2 pos);
D3DXVECTOR2 GetCreateMapPos(MapType type);
void DrawMapScreen(D3DXVECTOR2 pos, MapType type);
bool CheckCurrentCreateMap(D3DXVECTOR2 pos);
ScreenMap CheckMap(D3DXVECTOR2 pos);
bool CheckSquare(D3DXVECTOR2 target, D3DXVECTOR2 leftUpPos, D3DXVECTOR2 size);

void InitMapEditor() {
	textureIds[MAP_NONE] = ReserveTextureLoadFile("texture/MAP_NONE.png");
	textureIds[MAP_BLOCK_NONE] = ReserveTextureLoadFile("texture/MAP_BLOCK_NONE_ERASE.png");
	textureIds[MAP_BLOCK] = ReserveTextureLoadFile("texture/block02.png");
	textureIds[MAP_WALL] = ReserveTextureLoadFile("texture/wall.png");
	textureIds[MAP_ROCK] = ReserveTextureLoadFile("texture/brokenblock01.png");
	textureIds[MAP_GOAL] = ReserveTextureLoadFile("texture/warpblock32_64_anime.png");

}
void UninitMapEditor() {
	ReleaseTexture(textureIds, MAP_MAX);

}
void DrawMapEditor() {
	for (int i = MAP_BLOCK_NONE; i < MAP_MAX; i++) {
		auto mapType = (MapType)i;
		DrawMapScreen(GetCreateMapPos(mapType), mapType);
	}

	if (currentMap.map.type != MAP_NONE) {
		DrawMapScreen(currentMap.pos - D3DXVECTOR2(CREATE_MAP_WIDTH, CREATE_MAP_HEIGHT) / 2, currentMap.map.type);
	}
}
void UpdateMapEditor() {
	frame++;
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));

	if (TriggerInputLogger(MYVK_LEFT_CLICK)) {
		//bool isCurrentMap = CheckCurrentCreateMap(mousePos);
		ScreenMap matchedCreateMap = CheckCreateMap(mousePos);
		ScreenMap matchedMap = CheckMap(mousePos);
		//DebugPrintf("%d,", isCurrentMap);
		//DebugPrintf("%d %f %f,", matchedCreateMap.map.type, matchedCreateMap.pos.x, matchedCreateMap.pos.y);
		DebugPrintf("%d %f %f,", matchedMap.map.type, matchedMap.pos.x, matchedMap.pos.y);
		DebugPrintf("\n");

		//今持ってる
		if (currentMap.map.type != MAP_NONE) {
			if (matchedMap.map.type != MAP_NONE) {
				//チェンジ
				auto gamePos = ScreenToGamePos(matchedMap.pos);
				Map& map = GetMap(gamePos.y, gamePos.x);
				map = currentMap.map;
			}
			else {
				//離す
				currentMap.map.type = MAP_NONE;
			}
		}
		if (matchedCreateMap.map.type != MAP_NONE) {
			currentMap = matchedCreateMap;
		}

	}
	if (currentMap.map.type != MAP_NONE) {
		currentMap.pos = mousePos;
	}


	if (PressInputLogger(MYVK_CONTROL)&&TriggerInputLogger(MYVK_SAVE)) {
		MapExport("stage/edit/edit.map");

	}
	if (PressInputLogger(MYVK_CONTROL)&&TriggerInputLogger(MYVK_LOAD)) {
		MapImport("stage/edit/edit.map");
	}
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

bool CheckCurrentCreateMap(D3DXVECTOR2 pos) {
	if (currentMap.map.type == MAP_NONE) {
		return false;
	}
	return CheckSquare(pos, currentMap.pos, { CREATE_MAP_WIDTH,CREATE_MAP_HEIGHT });
}

ScreenMap CheckCreateMap(D3DXVECTOR2 pos) {
	for (int i = MAP_BLOCK_NONE; i < MAP_MAX; i++) {
		MapType type = (MapType)i;
		auto screenPos = GetCreateMapPos(type);
		if (CheckSquare(pos, screenPos, { CREATE_MAP_WIDTH,CREATE_MAP_HEIGHT })) {
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
	for (int i = 0; i < GetMapHeight(); i++) {
		for (int j = 0; j < GetMapWidth(); j++) {
			D3DXVECTOR2 gamePos = D3DXVECTOR2(j, i);
			auto screenPos = GameToScreenPos(gamePos);
			if (CheckSquare(pos, screenPos, { GAME_SPRITE_WHIDTH,GAME_SPRITE_HEIGHT })) {
				return { GetMap(i,j),screenPos };
			}
		}
	}

	return {};
}

bool CheckSquare(D3DXVECTOR2 target, D3DXVECTOR2 leftUpPos, D3DXVECTOR2 size) {
	return (leftUpPos.x < target.x) && (target.x < leftUpPos.x + size.x) &&
		(leftUpPos.y < target.y) && (target.y < leftUpPos.y + size.y);
}

void DrawMapScreen(D3DXVECTOR2 pos, MapType type) {

	D3DXVECTOR2 size = { CREATE_MAP_WIDTH,CREATE_MAP_HEIGHT };

	D3DXVECTOR2 tPos = { 0,0 };
	D3DXVECTOR2 tSize = { CREATE_MAP_TEXTURE_WIDTH,CREATE_MAP_TEXTURE_HEIGHT };

	if (type == MAP_WALL) {
		auto addDir = createWallDir + INTVECTOR2(1, 1);
		tPos = D3DXVECTOR2(
			addDir.x * CREATE_MAP_TEXTURE_WIDTH,               //0  1   2  
			addDir.y * CREATE_MAP_TEXTURE_HEIGHT               //0 0.3 0.6
		);
	}
	if (type == MAP_GOAL) {
		tPos.x = CREATE_MAP_TEXTURE_WIDTH * (frame / 8 % 8);
		tPos.y = CREATE_MAP_TEXTURE_HEIGHT;
	}

	DrawSprite(textureIds[type], pos, 10, size, tPos, tSize);
}