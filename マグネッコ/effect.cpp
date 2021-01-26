#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "myd3d.h"
#include "gameSrite.h"
#include "effect.h"
#include "trans.h"
#include <vector>

#define EFFECT_TEXTURE_WIDTH  (32)
#define EFFECT_TEXTURE_HEIGHT (32)

#define EFFECT_ANIMATION_MAX (45)

static int textureId = TEXTURE_INVALID_ID;

struct Effect {
	D3DXVECTOR2 pos;
	int frame;
};
static std::vector<Effect> effects;

void InitEffect()
{
	textureId = ReserveTextureLoadFile("texture/effect/explosion_anime.png");

	effects.clear();
}

void UninitEffect()
{
	ReleaseTexture(textureId);
}
void UpdateEffect()
{
	for (auto itr = effects.begin(); itr != effects.end();) {

		itr->frame++;
		if (itr->frame >= EFFECT_ANIMATION_MAX) {
			itr = effects.erase(itr);

		}

		else {
			itr++;
		}
	}
}

void DrawEffect()
{
	for (auto itr = effects.begin(); itr != effects.end(); itr++) {

		auto tPos = D3DXVECTOR2(
			EFFECT_TEXTURE_WIDTH * (itr->frame / 4 % 12),
			0
		);

		DrawGameSprite(textureId, itr->pos - D3DXVECTOR2(0.5, 0.5), 30, tPos, D3DXVECTOR2(EFFECT_TEXTURE_WIDTH, EFFECT_TEXTURE_HEIGHT));
	}
}

void CreateEffect(D3DXVECTOR2 pos)
{
	Effect effect;
	effect.pos = pos;
	effect.frame = 0;

	effects.push_back(effect);
}