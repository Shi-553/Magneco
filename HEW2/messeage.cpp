#include "messeage.h"
#include "font.h"
#include"myd3d.h"
#include "config.h"
#include "trans.h"
#include <stdio.h>
#include <mbstring.h>



#define FONT_WIDTH	(long)(15 * 1.2*fontScale.x)
#define FONT_HEIGHT	(long)(32 * 1.2*fontScale.y)

D3DXVECTOR2 fontScale;

static int fontCountMaxX ;//半角45文字まで
static int fontCountMaxY ;//10行まで

//横幅、大きくすると左右に伸びる
#define RECT_WIDTH (FONT_WIDTH*fontCountMaxX)
//高さ、大きくすると下に伸びる
#define RECT_HEIGHT (FONT_HEIGHT*fontCountMaxY)

//中心からどれだけ下にするか
static int rectAddY;

long fontMarginY;

void UpdateRect(RECT& rect);

static LPD3DXFONT font;
static INTVECTOR2 offset;
static LPD3DXSPRITE sprite;
static D3DCOLOR color;


void InitMesseage() {
	fontCountMaxX = 45;
	fontCountMaxY = 6;
	rectAddY = 75;
	fontScale = { 0,0 };
	fontMarginY = 0;

	SetFontScale(fontScale);
	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	D3DXCreateSprite(device, &sprite);

	color = D3DCOLOR_RGBA(255, 255, 255, 255);

}

void SetFontMargin(long margin) {
	fontMarginY = margin;
}
void SetFontRectAddY(int addY) {
	rectAddY=addY;
}
void SetFontScale(D3DXVECTOR2 scale) {
	fontScale = scale;
	if (font != NULL) {
		font->Release();
	}
	MyCreateFont(FONT_HEIGHT, FONT_WIDTH, &font);
}
void SetFontCountMax(int x, int y) {
	fontCountMaxX=x;
	fontCountMaxY=y;
}
void UninitMesseage() {
	sprite->Release();
	if (font != NULL) {
		font->Release();
	}
}

void ClearMesseageOffset() {
	offset = INTVECTOR2(0, 0);
}

void UpdateRect(RECT& rect) {
	rect = {
		(SCREEN_WIDTH - RECT_WIDTH) / 2 + offset.x * FONT_WIDTH,	         	// 左上のx座標
		SCREEN_HEIGHT / 2 + rectAddY + offset.y * (FONT_HEIGHT+ fontMarginY),				// 左上のy座標
		(SCREEN_WIDTH + RECT_WIDTH) / 2 ,		                                    // 右下のx座標
		SCREEN_HEIGHT / 2 + rectAddY + RECT_HEIGHT 		                        // 右下のy座標
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
		bool isM = (offset.x == fontCountMaxX);
		bool isMax = isM ||
			(offset.x == fontCountMaxX - 1 && _mbclen((BYTE*)&(buf[i])) == 2);

		if (isNewLine || isMax || isEnd) {
			if (isMax && !isM) {
				i++;
			}

			font->DrawTextA(sprite,
				&(buf[startIndex]),
				i - startIndex,
				&rc,
				DT_LEFT|DT_NOCLIP,//左寄せ
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