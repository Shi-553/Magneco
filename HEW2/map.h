#ifndef MAP_H
#define MAP_H

#include "myd3d.h"
#include "flyingObject.h"
#include "trans.h"



enum MapType
{
	MAP_NONE,    // 無効な値
	MAP_BLOCK_NONE,  // ブロックなし
	MAP_BLOCK,       // ブロック
	MAP_WALL,     // 壁
	MAP_ROCK,        // 岩
	MAP_GOAL,        // ゴール
	MAP_CHEST_CLOSE,        // 宝箱閉じてる
	MAP_CHEST_OPEN,        // 宝箱開いてる
	MAP_MAX
};

struct Map
{
	MapType type;
	INTVECTOR2 dir = INTVECTOR2::GetNone();
	int param=0;
};


void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

// 置いたところのマップタイプを変える
void MapChange(FlyingObject flyingobject);

// 4方向にブロックがあるかの判定
bool MapFourDirectionsJudgment(INTVECTOR2 pos);

// 位置を指定してマップタイプを返す
MapType GetMapType(INTVECTOR2 pos);

bool MapExport(const char* filename);
bool MapImport(const char* filename);

Map& GetMap(int y, int x);

int GetMapHeight();
int GetMapWidth();

bool CanGoNPCMapType(MapType type);
void OpenChest(INTVECTOR2 pos);
#endif // !MAP_H
