#ifndef MAP_H
#define MAP_H

#include "myd3d.h"
#include "flyingObject.h"
#include "trans.h"

#include <vector>
using namespace std;


enum MapType
{
	MAP_NONE,             // 無効な値
	MAP_BLOCK_NONE,       // ブロックなし
	MAP_BLOCK,            // 壊れるブロック
	MAP_CHAECKPOINT_ON,   // チェックポイントON
	MAP_CHAECKPOINT_OFF,  // チェックポイントOFF
	MAP_WALL,             // 壁
	MAP_ROCK,             // 岩
	MAP_GOAL,             // ゴール
	MAP_CHEST_CLOSED,        // 宝箱閉じてる
	MAP_CHEST_OPENED,        // 宝箱開いてる
	MAP_MAX
};

struct Map
{
	MapType type;
	INTVECTOR2 dir = INTVECTOR2::GetNone();
	int param = 0;
};


void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

// 置いたところのマップタイプを変える
void MapChange(FlyingObject flyingobject);

// 4方向にブロックがあるかの判定
bool MapFourDirectionsJudgment(INTVECTOR2& pos);

// 位置を指定してマップタイプを返す
MapType GetMapType(INTVECTOR2 pos);

bool MapExport(FILE* fp);
bool MapImport(FILE* fp);

Map* GetMap(INTVECTOR2 pos);

int GetMapHeight();
int GetMapWidth();
void SetMapHeight(int h);

void SetMapWidth(int w);

bool CanGoNPCMapType(MapType type);
void OpenChest(INTVECTOR2 pos);
int GetMapTextureId(MapType type);

bool IsBreakBlock(INTVECTOR2 pos, vector<INTVECTOR2>& v);
#endif // !MAP_H
