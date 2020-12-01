#include "enterBottonSprite.h"
#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "gameSrite.h"


#define ENTER_BOTTON_SPRITE_WIDTH 640
#define ENTER_BOTTON_SPRITE_HEIGHT 64
#define ENTER_BOTTON_SPRITE_ONE_WIDTH 64


static int enterBottonTexture;
static int anime_frame;


void InitEnterBottonSprite() {
	enterBottonTexture = ReserveTextureLoadFile("texture/entersozai_anime.png");

	LoadTexture();

	anime_frame = 0;

}

void UninitEnterBottonSprite() {
	ReleaseTexture(enterBottonTexture);

}

void UpdateEnterBottonSprite() {
	anime_frame++;
}

void DrawEnterBottonSprite() {
	auto tPos = D3DXVECTOR2(
		ENTER_BOTTON_SPRITE_ONE_WIDTH * (anime_frame / 10 % 10),
		0
	);

	DrawSprite(enterBottonTexture, { SCREEN_WIDTH / 2 + 230, SCREEN_HEIGHT / 2  + 150}, 30, { ENTER_BOTTON_SPRITE_ONE_WIDTH, ENTER_BOTTON_SPRITE_HEIGHT }, tPos, D3DXVECTOR2(ENTER_BOTTON_SPRITE_ONE_WIDTH, ENTER_BOTTON_SPRITE_HEIGHT));

}

