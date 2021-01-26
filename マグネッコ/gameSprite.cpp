#include "gameSrite.h"
#include "sprite.h"



void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize) {
	auto gameSpritePos = D3DXVECTOR2(
		GAME_SPRITE_ZERO_X + (pos.x * GAME_SPRITE_WHIDTH),
		GAME_SPRITE_ZERO_Y + (pos.y * GAME_SPRITE_HEIGHT));


	DrawSprite(textureId, gameSpritePos, z, GameToScreenSize(size), tPos, tSize);
}

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize) {
	auto gameSpritePos = D3DXVECTOR2(
		GAME_SPRITE_ZERO_X + (pos.x * GAME_SPRITE_WHIDTH),
		GAME_SPRITE_ZERO_Y + (pos.y * GAME_SPRITE_HEIGHT));

	auto gameSpriteSize = D3DXVECTOR2(GAME_SPRITE_WHIDTH, GAME_SPRITE_HEIGHT);

	DrawSprite(textureId, gameSpritePos, z, gameSpriteSize, tPos, tSize);
}

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z) {

	auto gameSpriteSize = D3DXVECTOR2(GAME_SPRITE_WHIDTH, GAME_SPRITE_HEIGHT);

	DrawSprite(textureId, GameToScreenPos(pos), z, gameSpriteSize);
}

void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size) {

	DrawSprite(textureId, GameToScreenPos(pos), z, GameToScreenSize(size));
}
void DrawGameSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, D3DXVECTOR2 cPos, float rad) {


	DrawSprite(textureId, GameToScreenPos(pos), z, GameToScreenSize(size), tPos, tSize, GameToScreenSize(cPos),rad);
}

D3DXVECTOR2 ScreenToGamePos(D3DXVECTOR2& screenPos) {
	return D3DXVECTOR2(
		(screenPos.x - GAME_SPRITE_ZERO_X) / GAME_SPRITE_WHIDTH,
		(screenPos.y - GAME_SPRITE_ZERO_Y) / GAME_SPRITE_HEIGHT
	);
}
D3DXVECTOR2 GameToScreenPos(D3DXVECTOR2& gamePos) {
	return D3DXVECTOR2(
		GAME_SPRITE_ZERO_X + (gamePos.x * GAME_SPRITE_WHIDTH),
		GAME_SPRITE_ZERO_Y + (gamePos.y * GAME_SPRITE_HEIGHT)
	);
}

D3DXVECTOR2 ScreenToGameSize(D3DXVECTOR2& screenSize) {
	return D3DXVECTOR2(
		screenSize.x / GAME_SPRITE_WHIDTH,
		screenSize.y / GAME_SPRITE_HEIGHT
	);
}
D3DXVECTOR2 GameToScreenSize(D3DXVECTOR2& gameSize) {
	return D3DXVECTOR2(
		gameSize.x * GAME_SPRITE_WHIDTH,
		gameSize.y * GAME_SPRITE_HEIGHT
	);
}