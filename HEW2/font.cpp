
#include "font.h"
#include <stdio.h>
#include "debugPrintf.h"

static LPD3DXFONT	pFont=NULL;
static auto fontPath = "./font/PixelMplus-20130602/PixelMplus10-Regular.ttf";

LPD3DXFONT GetFont() {
		return pFont;
}


void InitFont() {
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) <= 0) {
		DebugPrintf("error AddFontResourceEx\n");
		return;
	}

	auto device = GetD3DDevice();

	auto hr = D3DXCreateFont(
		device,				// Direct3Dデバイス
		32,						// 高さ
		15,						// 幅
		FW_REGULAR,				// フォントの太さ 普通
		NULL,					// 下線
		FALSE,					// 斜体
		DEFAULT_CHARSET,		// 文字セット
		OUT_DEFAULT_PRECIS,		// 出力制度は普通
		PROOF_QUALITY,			// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		"PixelMplus10",	// フォント名
		&pFont);					

	if (FAILED(hr))
	{
		DebugPrintf("error font\n");
		return;
	}

	//ここで使う文字をロードしておくとよりよい
	//pFont->PreloadText("",sizeof(""));
}
void UninitFont() {
	if (pFont != NULL) {
		pFont->Release();
		pFont = NULL;
	}
	if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL) <= 0) {
		DebugPrintf("error RemoveFontResourceEx\n");
	}
}

//void DrawFont() {
//	auto font = GetFont();
//	LPD3DXSPRITE sprite = NULL;
//	D3DXCreateSprite(d3dDevice, &sprite);
//
//	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
//
//	RECT rc = {
//	SCREEN_WIDTH / 2 - 400,		// 左上のx座標
//	SCREEN_HEIGHT / 2 + 250,		// 左上のy座標
//	SCREEN_WIDTH / 2 + 400,		// 右下のx座標
//	SCREEN_HEIGHT / 2 + 350		// 右下のy座標
//	};
//	auto hdc = font->GetDC();
//
//	font->DrawTextA(sprite, "unicode support", -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
//	sprite->End();
//	sprite->Release();
//}