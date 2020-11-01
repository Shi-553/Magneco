#include "gameSrite.h"
#include "sprite.h"
#include "config.h"
#include "map.h"

#define GAME_SPRITE_WHIDTH (50)
#define GAME_SPRITE_HEIGHT (50)

#define GAME_SPRITE_ZERO_X (SCREEN_WIDTH  /2 - (GAME_SPRITE_WHIDTH*MAPCHIP_WIDTH ) /2)
#define GAME_SPRITE_ZERO_Y (SCREEN_HEIGHT /2 - (GAME_SPRITE_HEIGHT*MAPCHIP_HEIGHT) /2)

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z) {
	auto gameSpritePos = D3DXVECTOR2(
		GAME_SPRITE_ZERO_X + (pos.x * GAME_SPRITE_WHIDTH),
		GAME_SPRITE_ZERO_Y + (pos.y * GAME_SPRITE_HEIGHT));

	auto gameSpriteSize = D3DXVECTOR2(GAME_SPRITE_WHIDTH, GAME_SPRITE_HEIGHT);

	DrawSprite(textureId, gameSpritePos, z, gameSpriteSize);
}