#pragma once

enum EffectType {
	EFFECT_EXPLOSION,
	EFFECT_BLOCK_EXPLOSION,
	EFFECT_MAX
};
void InitEffect();
void UninitEffect();
void UpdateEffect();
void DrawEffect();

void CreateEffect(EffectType type,D3DXVECTOR2 pos);
