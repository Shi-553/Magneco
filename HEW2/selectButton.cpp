#include "selectButton.h"
#include "sprite.h"
#include <vector>

#define FRAME_WIDTH 0
#define FRAME_HEIGHT 0

static std::vector<Button*> buttons;
static int selectedIndex;

static int selectFrameTextureId = TEXTURE_INVALID_ID;

void InitSelectButton() {
	selectedIndex = 0;
	selectFrameTextureId = ReserveTextureLoadFile("texture/select.png");
	buttons.clear();
}


void DrawSelectButton() {
	if (buttons.empty()) {
		return;
	}
	//ƒtƒŒ[ƒ€‚Ì•`‰æ
	auto selected = buttons[selectedIndex];
	DrawSprite(selectFrameTextureId, selected->pos-D3DXVECTOR2(FRAME_WIDTH/2,FRAME_HEIGHT/2), selected->z, selected->size + D3DXVECTOR2(FRAME_WIDTH, FRAME_HEIGHT));


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

void EnterSelectButton() {
	buttons[selectedIndex]->pressedCallback();
}