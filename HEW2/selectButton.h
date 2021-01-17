#pragma once
#include "myd3d.h"
#include "texture.h"
#include "InputLogger.h"

typedef struct Button {
	D3DXVECTOR2 pos ;
	D3DXVECTOR2 size;

	int textureId=TEXTURE_INVALID_ID;
	int pressedTextureId=TEXTURE_INVALID_ID;

	void (*pressedCallback)()=NULL;
	void (*triggeredCallback)()=NULL;
	void (*releasedCallback)()=NULL;

};

void InitSelectButton();
void DrawSelectButton();
void UpdateSelectButton();
void UninitSelectButton();


void AddSelectButton(Button& button);
void SetSelectButtonKey(VirtualKey enterKey, VirtualKey forwardKey, VirtualKey backKey);

void SetSelectButtonFrame(int frame);
void SetSelectButtonForward(int forward, int forwardPressed, D3DXVECTOR2 pos, D3DXVECTOR2 size);
void SetSelectButtonBack(int back, int backPressed, D3DXVECTOR2 pos, D3DXVECTOR2 size);