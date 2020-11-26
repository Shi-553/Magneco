#pragma once
#include "myd3d.h"
#include "map.h"
#include "config.h"

#define GAME_SPRITE_WHIDTH (50)
#define GAME_SPRITE_HEIGHT (50)


#define GAME_SPRITE_ZERO_X (SCREEN_WIDTH  /2 - (GAME_SPRITE_WHIDTH*MAPCHIP_WIDTH ) /2)
#define GAME_SPRITE_ZERO_Y (SCREEN_HEIGHT /2 - (GAME_SPRITE_HEIGHT*MAPCHIP_HEIGHT) /2)

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize);

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize);

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z);


