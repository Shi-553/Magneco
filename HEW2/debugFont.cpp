#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "texture.h"
#include <string.h>
#include <stdio.h>
#include "debugFont.h"
#include "sprite.h"

#define MIN_CHAR_CODE (int)(' ') // 32
#define MAX_CHAR_CODE (int)('~') // 126

#define ONE_LINE_COUNT 16

// SKT
int textureId;

void InitDebugFont() {
	textureId = ReserveTextureLoadFile("texture/debug_font_32x64.png");
}

void UninitDebugFont() {
	ReleaseTexture(textureId);
}

DEBUG_FONT_STATE DrawDebugFont(DEBUG_FONT_STATE* state, const char* str, va_list argp) {
	DEBUG_FONT_STATE s;

#if defined(_DEBUG)||defined(DEBUG)
	if (str == NULL) {
		return s;
	}


	if (state != NULL) {
		s = *state;
	}

	if (s.size.x == 0 && s.size.y == 0) {
		s.size.x = FONT_WIDTH;
		s.size.y = FONT_HEIGHT;
	}

	if (s.size.x == 0) {
		s.size.x = s.size.y * (float)FONT_WIDTH / FONT_HEIGHT;
	}

	if (s.size.y == 0) {
		s.size.y = s.size.x * (float)FONT_HEIGHT / FONT_WIDTH;
	}

	float w = s.size.x * s.scale;
	float h = s.size.y * s.scale;


	long long length = _vscprintf(str, argp);
	char* buf = (char*)malloc(sizeof(char*) * (length + 1));
	
	vsprintf(buf, str, argp);

	for (int i = 0; i < length; i++)
	{
		if (buf[i] == '\n') {
			s.charOffsetX = 0;
			s.charOffsetY ++;
			continue;
		}

		int code = (int)buf[i];

		if (code < MIN_CHAR_CODE || MAX_CHAR_CODE < code) {
			code = (int)'?';
		}

		float x = s.pos.x + w * s.charOffsetX;
		float y = s.pos.y + h * s.charOffsetY;

		float tx = (code - MIN_CHAR_CODE) % ONE_LINE_COUNT * FONT_WIDTH;
		float ty = (code - MIN_CHAR_CODE) / ONE_LINE_COUNT * FONT_HEIGHT;

		DrawSprite(textureId, { x,y }, s.z, { w,h }, { tx,ty }, { FONT_WIDTH,FONT_HEIGHT });


		s.charOffsetX++;
	}

	free(buf);


	return s;

#else
	UNREFERENCED_PARAMETER(s);
	UNREFERENCED_PARAMETER(str);

	return s;
#endif
}

DEBUG_FONT_STATE DrawDebugFont(DEBUG_FONT_STATE* state, const char* str, ...) {
	va_list argp;
	va_start(argp, str);

	DEBUG_FONT_STATE ret= DrawDebugFont(state, str, argp);

	va_end(argp);

	return ret;

}
