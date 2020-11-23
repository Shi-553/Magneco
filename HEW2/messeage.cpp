#include "messeage.h"
#include "font.h"
#include"myd3d.h"
#include "config.h"
#include <d3dx9.h>
#include "trans.h"


LPD3DXFONT font;
INTVECTOR2 offset_pos;
LPD3DXSPRITE sprite;


void InitMesseage() {
	font = GetFont();
	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	D3DXCreateSprite(device, &sprite);



}

void UninitMesseage() {
	sprite->Release();

}

void UpdateMesseage() {
	offset_pos = INTVECTOR2(0, 0);

}


void DrawMesseage(const char* str, bool red) {

	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	RECT rc = {
	SCREEN_WIDTH / 2 - 400 + offset_pos.x * 16,		// 左上のx座標
	SCREEN_HEIGHT / 2 + 250 + offset_pos.y * 32,	// 左上のy座標
	SCREEN_WIDTH / 2 + 400 + offset_pos.x * 16,		// 右下のx座標
	SCREEN_HEIGHT / 2 + 350 + offset_pos.y * 32		// 右下のy座標
	};

	if (red) {
		font->DrawTextA(sprite, str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
	}
	else {
		font->DrawTextA(sprite, str, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
	sprite->End();

	offset_pos.x += strlen(str);


}
