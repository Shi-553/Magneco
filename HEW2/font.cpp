
#include "font.h"
#include <stdio.h>
#include "debugPrintf.h"

static auto fontPath = "./font/PixelMplus-20130602/PixelMplus10-Regular.ttf";

void InitFont() {
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) <= 0) {
		DebugPrintf("error AddFontResourceEx\n");
		return;
	}


	//ここで使う文字をロードしておくと効率いい
	//pFont->PreloadText("",sizeof(""));
}
void UninitFont() {
	if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL) <= 0) {
		DebugPrintf("error RemoveFontResourceEx\n");
	}
}

void CreateFont(int height, int width, LPD3DXFONT* font) {
	auto device = GetD3DDevice();

	auto hr = D3DXCreateFont(
		device,				
		height,						// 高さ
		width,						// 幅
		FW_REGULAR,				
		NULL,					
		FALSE,					
		DEFAULT_CHARSET,		// 文字コード
		OUT_DEFAULT_PRECIS,		
		PROOF_QUALITY,			
		FIXED_PITCH | FF_SCRIPT,	//ピッチとファミリ
		"PixelMplus10",	// 使うフォント
		font);

	if (FAILED(hr))
	{
		DebugPrintf("error font\n");
		return;
	}
}