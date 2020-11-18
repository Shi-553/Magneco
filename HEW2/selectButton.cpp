#include "selectButton.h"
#include "sprite.h"
#include <vector>

#define FRAME_WIDTH 20
#define FRAME_HEIGHT 20

static std::vector<Button*> buttons;
static int selectedIndex;

static int selectFrameTextureId = TEXTURE_INVALID_ID;

void InitSelectButton() {
	selectedIndex = 0;
	selectFrameTextureId = ReserveTextureLoadFile("texture/selectFrame.png");
	buttons.clear();
}


void DrawSelectButton() {
	if (buttons.empty()) {
		return;
	}
	//ƒtƒŒ[ƒ€‚Ì•`‰æ
	auto selected = buttons[selectedIndex];
	SetSpriteColor(selected->frameColor);
	DrawSprite(selectFrameTextureId, selected->pos-D3DXVECTOR2(FRAME_WIDTH/2,FRAME_HEIGHT/2), selected->z, selected->size + D3DXVECTOR2(FRAME_WIDTH, FRAME_HEIGHT));

	SetSpriteColor(D3DCOLOR_RGBA(255,255,255,255));

	//ƒ{ƒ^ƒ“‚Ì•`‰æ
	for (auto itr = buttons.begin(); itr != buttons.end(); itr++) {
		DrawSprite((*itr)->textureId, (*itr)->pos, (*itr)->z, (*itr)->size);
	}

}

void UninitSelectButton() {
	ReleaseTexture(selectFrameTextureId);
	buttons.clear();
}

void AddButton(Button* button) {
	buttons.push_back(button);
}

void ForwardSelectButton() {
	if (buttons.size()-1 <= selectedIndex) {
		return;
	}
	selectedIndex++;
}

void BackSelectButton() {
	if (selectedIndex <= 0) {
		return;
	}
	selectedIndex--;
}

void TriggerSelectButton() {
	buttons[selectedIndex]->triggeredCallback();
}

void ReleaseSelectButton() {
	buttons[selectedIndex]->releasedCallback();
}

void PressSelectButton() {
	buttons[selectedIndex]->pressedCallback();
}