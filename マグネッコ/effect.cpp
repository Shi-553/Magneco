#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "myd3d.h"
#include "gameSrite.h"
#include "effect.h"
#include "trans.h"
#include <vector>

#define EFFECT_WIDTH  (32)
#define EFFECT_HEIGHT (32)

#define EFFECT_ANIMATION_MAX (45)

static int textureId = TEXTURE_INVALID_ID;

struct Effect {
	D3DXVECTOR2 pos;
	int frame;
	EffectType type;
};
static std::vector<Effect> effects;

struct EffectTexture {
	int textureId;
	int xCount;
	INTVECTOR2 tSize;
};
static EffectTexture textures[EFFECT_MAX];

void InitEffect()
{
	textures[EFFECT_EXPLOSION] = { ReserveTextureLoadFile("texture/effect/explosion_anime.png"),
		12,INTVECTOR2(32,32) };
	textures[EFFECT_BLOCK_EXPLOSION] = { ReserveTextureLoadFile("texture/effect/blockExplosion_solo_64×64.png"),
		14,INTVECTOR2(64,64) };

	effects.clear();
}

void UninitEffect()
{
	for (auto& texture : textures) {
		ReleaseTexture(texture.textureId);
	}
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
		auto texture = textures[itr->type];

		auto tPos = D3DXVECTOR2(
			texture.tSize.x * (itr->frame / (int)ceilf(EFFECT_ANIMATION_MAX/ (float)texture.xCount) % texture.xCount),
			0
		);

		DrawGameSprite(texture.textureId, itr->pos - D3DXVECTOR2(0.5, 0.5), 30, tPos, texture.tSize.ToD3DXVECTOR2());
	}
}

void CreateEffect(EffectType type, D3DXVECTOR2 pos)
{
	Effect effect;
	effect.pos = pos;
	effect.frame = 0;
	effect.type = type;

	effects.push_back(effect);
}