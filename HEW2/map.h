#ifndef MAP_H
#define MAP_H

#include "myd3d.h"
#include "flyingObject.h"

enum MapType
{
	MAP_NONE,    // 無効な値
	MAP_BLOCK_NONE,  // ブロックなし
	MAP_BLOCK,       // ブロック
	MAP_WALL,        // 壁
	MAP_ROCK,        // 岩
	MAP_GOAL         // ゴール
};

#define MAPCHIP_WIDTH  (10)
#define MAPCHIP_HEIGHT (10)


void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);

// 置いたところのマップタイプを変える
void MapChange(FlyingObject flyingobject);

// 位置を指定してマップタイプを返す
MapType GetMapType(D3DXVECTOR2 pos);

#endif // !MAP_H
