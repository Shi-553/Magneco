#include "messeage.h"
#include "font.h"
#include"myd3d.h"
#include "config.h"
#include <d3dx9.h>


LPD3DXFONT font;
int offset_pos;


void InitMesseage() {
	font = GetFont();




}

void UninitMesseage() {

}

void UpdateMesseage() {

}


void DrawMesseage() {
	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	LPD3DXSPRITE sprite = NULL;
	D3DXCreateSprite(device, &sprite);

	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	RECT rc = {
	SCREEN_WIDTH / 2 - 400,		// 左上のx座標
	SCREEN_HEIGHT / 2 + 250,		// 左上のy座標
	SCREEN_WIDTH / 2 + 400,		// 右下のx座標
	SCREEN_HEIGHT / 2 + 350		// 右下のy座標
	};
	auto hdc = font->GetDC();

	font->DrawTextA(sprite, "unicode support", -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	sprite->End();
	sprite->Release();

}
