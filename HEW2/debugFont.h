#ifndef _DEBUG_FONT_H
#define _DEBUG_FONT_H
#include "myd3d.h"

#define FONT_WIDTH 32
#define FONT_HEIGHT 64

typedef struct DEBUG_FONT_STATE {
	D3DXVECTOR2 pos = {0,0};
	float z = 0;
	D3DXVECTOR2 size = {0,0};


	int charOffsetX=0, charOffsetY = 0;
	float scale=1;

};

void InitDebugFont();
void UninitDebugFont();

DEBUG_FONT_STATE DrawDebugFont(DEBUG_FONT_STATE* state, const char* str, va_list argp);
DEBUG_FONT_STATE DrawDebugFont(DEBUG_FONT_STATE* state, const char* str, ...);


#endif // !_DEBUG_FONT_H
