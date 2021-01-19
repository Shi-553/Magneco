#include "messeage.h"
#include "font.h"
#include"myd3d.h"
#include "config.h"
#include "trans.h"
#include <stdio.h>
#include <mbstring.h>



#define FONT_WIDTH	(15 * 1.2*fontScale.x)
#define FONT_HEIGHT	(32 * 1.2*fontScale.y)


Message::Message(D3DXVECTOR2 scale) {
	fontScale = scale;
	fontPos = { 0,0 };
	endPos = { 0,0 };
	fontMarginY = 0;
	format = DT_NOCLIP;

	SetScale(fontScale);
	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	D3DXCreateSprite(device, &sprite);

	color = D3DCOLOR_RGBA(255, 255, 255, 255);

}
Message::~Message() {
	if (sprite != NULL) {
		sprite->Release();
		sprite = NULL;
	}
	if (font != NULL) {
		font->Release();
		font = NULL;
	}
}

void Message::SetMargin(long margin) {
	fontMarginY = margin;
}
void Message::SetScale(D3DXVECTOR2 scale) {
	fontScale = scale;
	if (font != NULL) {
		font->Release();
		font = NULL;
	}
	MyCreateFont(FONT_HEIGHT, FONT_WIDTH, &font);
}
void Message::SetPos(D3DXVECTOR2 pos) {
	fontPos = pos;
	if (fontPos.x == -1) {
		fontPos.x = 0;
	}
	if (fontPos.y == -1) {
		fontPos.y = 0;
	}
}
void Message::SetEndPos(D3DXVECTOR2 pos) {
	endPos = pos;
	if (endPos.x == -1) {
		endPos.x = SCREEN_WIDTH;
	}
	if (endPos.y == -1) {
		endPos.y = SCREEN_HEIGHT;
	}
}
void Message::SetFormat(int f) {
	format = f ;
}
D3DXVECTOR2 Message::GetOneSize() {
	return D3DXVECTOR2(FONT_WIDTH,  FONT_HEIGHT);
}


void Message::ClearOffset() {
	offset = INTVECTOR2(0, 0);
}

void Message::UpdateRect(RECT& rect,const char* str) {
	rect.left = fontPos.x + offset.x * FONT_WIDTH;// 左上のx座標
	rect.top = fontPos.y + offset.y * (FONT_HEIGHT + fontMarginY);				// 左上のy座標
	rect.right = endPos.x- offset.x * FONT_WIDTH;	         	                                 // 右下のx座標
	rect.bottom = endPos.y- offset.y * (FONT_HEIGHT + fontMarginY);		                        // 右下のy座標

}

void Message::SetColor(D3DCOLOR c) {
	color = c;
}

void Message::Draw(const char* str, va_list argp) {
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
	UpdateRect(rc, str);

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
				format,
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

			UpdateRect(rc, str);


			continue;
		}

		offset.x++;
	}

	sprite->End();

	free(buf);

}

void Message::Draw(const char* str, ...) {
	va_list argp;
	va_start(argp, str);

	Draw(str, argp);

	va_end(argp);
}