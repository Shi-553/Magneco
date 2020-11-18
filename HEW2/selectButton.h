#pragma once
#include "myd3d.h"
#include "texture.h"

typedef struct Button {
	D3DXVECTOR2 pos;
	int z=100;
	D3DXVECTOR2 size;
	int textureId=TEXTURE_INVALID_ID;
	void (*pressedCallback)()=NULL;
	void (*triggeredCallback)()=NULL;
	void (*releasedCallback)()=NULL;

};

void InitSelectButton();
void DrawSelectButton();
void UninitSelectButton();


void AddButton(Button* button);

void ForwardSelectButton();

void BackSelectButton();
void TriggerSelectButton();
void PressSelectButton();
void ReleaseSelectButton();