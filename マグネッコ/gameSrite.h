#pragma once
#include "myd3d.h"
#include "map.h"
#include "config.h"

#define GAME_SPRITE_WHIDTH (50)
#define GAME_SPRITE_HEIGHT (50)


#define GAME_SPRITE_ZERO_X (SCREEN_WIDTH  /2 - (GAME_SPRITE_WHIDTH*GetMapWidth() ) /2)
#define GAME_SPRITE_ZERO_Y (SCREEN_HEIGHT /2 - (GAME_SPRITE_HEIGHT*GetMapHeight()) /2)

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize);

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize);

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z);

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size);
void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, D3DXVECTOR2 cPos, float rad);

D3DXVECTOR2 ScreenToGamePos(const D3DXVECTOR2& screenPos);
D3DXVECTOR2 GameToScreenPos(const D3DXVECTOR2& gamePos);

D3DXVECTOR2 ScreenToGameSize(const D3DXVECTOR2& screenSize);
D3DXVECTOR2 GameToScreenSize(const D3DXVECTOR2& gameSize);