#pragma once
#include "myd3d.h"
#include "texture.h"

typedef struct Button {
	D3DXVECTOR2 pos;
	int z=100;
	D3DXVECTOR2 size;
	int textureId=TEXTURE_INVALID_ID;
	void (*pressedCallback)()=NULL;

	D3DCOLOR frameColor=D3DCOLOR_RGBA(255,255,255,255);
};

void InitSelectButton();
void DrawSelectButton();
void UninitSelectButton();


void AddButton(Button* button);

void ForwardSelectButton();

void BackSelectButton();
void EnterSelectButton();