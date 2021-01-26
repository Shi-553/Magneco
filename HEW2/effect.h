#ifndef EFFECT_H_
#define EFFECT_H_

#include <vector>
#include "trans.h"

typedef struct Effect_Tag {
	D3DXVECTOR2 effectPos;
	int frame;
}Effect;

void InitEffect();
void UninitEffect();
void UpdateEffect();
void DrawEffect();


#endif // !EFFECT_H_
