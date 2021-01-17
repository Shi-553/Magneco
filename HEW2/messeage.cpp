#include "messeage.h"
#include "font.h"
#include"myd3d.h"
#include "config.h"
#include "trans.h"
#include <stdio.h>
#include <mbstring.h>



#define FONT_WIDTH	(long)(15 * 1.2*fontScale.x)
#define FONT_HEIGHT	(long)(32 * 1.2*fontScale.y)

static D3DXVECTOR2 fontScale;
static D3DXVECTOR2 fontPos;


long fontMarginY;

void UpdateRect(RECT& rect);

static LPD3DXFONT font;
static INTVECTOR2 offset;
static LPD3DXSPRITE sprite;
static D3DCOLOR color;


void InitMesseage() {
	fontScale = { 0,0 };
	fontPos = { 0,0 };
	fontMarginY = 0;

	SetFontScale(fontScale);
	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	D3DXCreateSprite(device, &sprite);

	color = D3DCOLOR_RGBA(255, 255, 255, 255);

}

void SetFontMargin(long margin) {
	fontMarginY = margin;
}
void SetFontScale(D3DXVECTOR2 scale) {
	fontScale = scale;
	if (font != NULL) {
		font->Release();
		font = NULL;
	}
	MyCreateFont(FONT_HEIGHT, FONT_WIDTH, &font);
}
void SetFontPos(D3DXVECTOR2 pos) {
	fontPos = pos;
}

void UninitMesseage() {
	sprite->Release();
	if (font != NULL) {
		font->Release();
		font = NULL;
	}
}

void ClearMesseageOffset() {
	offset = INTVECTOR2(0, 0);
}

void UpdateRect(RECT& rect) {
	rect = {
		(LONG)fontPos.x + offset.x * FONT_WIDTH,	         	// 左上のx座標
		(LONG)fontPos.y + offset.y * (FONT_HEIGHT+ fontMarginY),				// 左上のy座標
		0,                                 // 右下のx座標
		0,		                        // 右下のy座標
	};
}

void SetMessageColor(D3DCOLOR c) {
	color = c;
}

void DrawMesseage(const char* str, va_list argp) {
	int length = _vscprintf(str, argp) + 1;// +1 '\0'
	if (length <= 1) {
		return;
	}
	char* buf = (char*)malloc(sizeof(char) * length);
	if (buf == NULL) {
		return;
	}

	vsprintf_s(buf, length, str, argp);

	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	RECT rc;
	UpdateRect(rc);

	int startIndex = 0;

	for (int i = 0; i < length; i++) {//終了文字の1つ前まで
		//改行か
		bool isNewLine = buf[i] == '\n';
		//最後か
		bool isEnd = i == length - 1;


		if (isNewLine || isEnd) {

			font->DrawTextA(sprite,
				&(buf[startIndex]),
				i - startIndex,
				&rc,
				DT_LEFT|DT_NOCLIP,//左寄せ
				color);

			if (isNewLine) {
				offset.y++;
				offset.x = 0;
			}

			if (isNewLine) {
				startIndex = i + 1;//改行文字ならそれをスキップ
			}
			else {
				startIndex = i;
			}

			UpdateRect(rc);


			continue;
		}

		offset.x++;
	}

	sprite->End();

	free(buf);

}

void DrawMesseage(const char* str, ...) {
	va_list argp;
	va_start(argp, str);

	DrawMesseage(str, argp);

	va_end(argp);
}


void TestMessage() {
#ifdef _DEBUG
	InitMesseage();

	ClearMesseageOffset();

	SetMessageColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	DrawMesseage("1111111111\n11111111111%s11111111", "aあああ");
	DrawMesseage("あいう%dえ6", 1515151);
	DrawMesseage("あいうえ6");
	DrawMesseage("あいうえ6");

	SetMessageColor(D3DCOLOR_RGBA(0, 255, 255, 255));
	DrawMesseage("あいうえ6");
	DrawMesseage("あいう%.1fえ6", 1.54f);
	DrawMesseage("あいうえ5\n\n");

	SetMessageColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	DrawMesseage("あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6あいうえ6");

	UninitMesseage();
#endif
}