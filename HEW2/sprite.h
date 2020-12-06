#ifndef _SPRITE_H
#define _SPRITE_H
#include "myd3d.h"

void InitSprite();
void UninitSprite();

void SetSpriteColor(D3DCOLOR color);

void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, bool  flipLR = false);
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, bool  flipLR = false);
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, bool  flipLR = false);
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, D3DXVECTOR2 cPos, float rad, bool  flipLR = false);
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, D3DXVECTOR2 cPos, D3DXMATRIX* matrix, bool  flipLR = false);

#endif // !_SPRITE_H
