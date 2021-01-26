#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "myd3d.h"
#include "gameSrite.h"
#include "flyingObject.h"
#include "effect.h"
#include "judge.h"

#define EFFECT_TEXTURE_WIDTH  (32)
#define EFFECT_TEXTURE_HEIGHT (32)


static int textureId = TEXTURE_INVALID_ID;

static Effect effect;

void InitEffect()
{
	textureId = ReserveTextureLoadFile("texture/effect/explosion_anime.png");

	effect.effectPos = D3DXVECTOR2(0, 0);
	effect.frame = 0;
}

void UninitEffect()
{
	ReleaseTexture(textureId);
}

void UpdateEffect()
{
	effect.frame++;
}

void DrawEffect()
{
		auto tPos = D3DXVECTOR2(
			EFFECT_TEXTURE_WIDTH * (effect.frame / 12 % 12),
			0
		);

		DrawGameSprite(textureId, effect.effectPos, 30, D3DXVECTOR2(EFFECT_TEXTURE_WIDTH, EFFECT_TEXTURE_HEIGHT), tPos, D3DXVECTOR2(EFFECT_TEXTURE_WIDTH, EFFECT_TEXTURE_HEIGHT));
}
