#include "gameSrite.h"
#include "sprite.h"





void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize) {
	auto gameSpritePos = D3DXVECTOR2(
		GAME_SPRITE_ZERO_X + (pos.x * GAME_SPRITE_WHIDTH),
		GAME_SPRITE_ZERO_Y + (pos.y * GAME_SPRITE_HEIGHT));

	auto gameSpriteSize = D3DXVECTOR2(GAME_SPRITE_WHIDTH, GAME_SPRITE_HEIGHT);

	DrawSprite(textureId, gameSpritePos, z, gameSpriteSize, tPos, tSize);
}

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z) {
	auto gameSpritePos = D3DXVECTOR2(
		GAME_SPRITE_ZERO_X + (pos.x * GAME_SPRITE_WHIDTH),
		GAME_SPRITE_ZERO_Y + (pos.y * GAME_SPRITE_HEIGHT));

	auto gameSpriteSize = D3DXVECTOR2(GAME_SPRITE_WHIDTH, GAME_SPRITE_HEIGHT);

	DrawSprite(textureId, gameSpritePos, z, gameSpriteSize);
}

void ScreenToGame(D3DXVECTOR2* screen,D3DXVECTOR2* game) {
	game->x =GAME_SPRITE_ZERO_X + (screen->x * GAME_SPRITE_WHIDTH);
	game->y = GAME_SPRITE_ZERO_Y + (screen->y * GAME_SPRITE_HEIGHT);
}
void GameToScreen(D3DXVECTOR2* game, D3DXVECTOR2* screen) {
	screen->x = (game->x- GAME_SPRITE_ZERO_X)/ GAME_SPRITE_WHIDTH;
	screen->y = (game->y - GAME_SPRITE_ZERO_Y) / GAME_SPRITE_HEIGHT;
}