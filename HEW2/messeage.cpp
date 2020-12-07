#include "messeage.h"
#include "font.h"
#include"myd3d.h"
#include "config.h"
#include "trans.h"
#include <stdio.h>
#include <mbstring.h>


#define FONT_WIDTH	(15)
#define FONT_HEIGHT	(32)

#define FONT_COUNT_MAX_WIDTH (30)//半角30文字まで
#define FONT_COUNT_MAX_HEIGHT (10)//10行まで

//横幅、大きくすると左右に伸びる
#define RECT_WIDTH (FONT_WIDTH*FONT_COUNT_MAX_WIDTH)
//高さ、大きくすると下に伸びる
#define RECT_HEIGHT (FONT_HEIGHT*FONT_COUNT_MAX_HEIGHT)

//中心からどれだけ下にするか
#define RECT_ADD_Y (0)

void UpdateRect(RECT& rect);

static LPD3DXFONT font;
static INTVECTOR2 offset;
static LPD3DXSPRITE sprite;
static D3DCOLOR color;


void InitMesseage() {
	CreateFont(FONT_HEIGHT, FONT_WIDTH, &font);
	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	D3DXCreateSprite(device, &sprite);

	color = D3DCOLOR_RGBA(255, 255, 255, 255);

}

void UninitMesseage() {
	sprite->Release();
	font->Release();
}

void ClearMesseageOffset() {
	offset = INTVECTOR2(0, 0);
}

void UpdateRect(RECT& rect) {
	rect = {
		(SCREEN_WIDTH - RECT_WIDTH) / 2 + offset.x * FONT_WIDTH,	         	// 左上のx座標
		SCREEN_HEIGHT / 2 + RECT_ADD_Y + offset.y * FONT_HEIGHT,				// 左上のy座標
		(SCREEN_WIDTH + RECT_WIDTH) / 2 ,		                                    // 右下のx座標
		SCREEN_HEIGHT / 2 + RECT_ADD_Y + RECT_HEIGHT 		                        // 右下のy座標
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
		//最大文字数かどうか（最大か、最大から1つ前で2バイト文字）
		bool isMax = (offset.x == FONT_COUNT_MAX_WIDTH) ||
			(offset.x == FONT_COUNT_MAX_WIDTH - 1 && _mbclen((BYTE*)&(buf[i])) == 2);

		if (isNewLine || isMax || isEnd) {
			font->DrawTextA(sprite,
				&(buf[startIndex]),
				i - startIndex,
				&rc,
				DT_LEFT,//左寄せ
				color);

			if (isNewLine || isMax) {
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

			if (!isEnd &&!isNewLine &&isMax) {
				offset.x++;//↑このときここで1個右へずらすとなぜかうまくいく・・・
			}

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