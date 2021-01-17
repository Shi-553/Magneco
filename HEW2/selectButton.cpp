#include "selectButton.h"
#include "sprite.h"
#include <vector>


static std::vector<Button> buttons;
static int selectedIndex;

static int selectFrameTextureId = TEXTURE_INVALID_ID;

static bool isPressed = false;

void InitSelectButton() {
	UninitSelectButton();

	selectedIndex = 0;
	selectFrameTextureId = ReserveTextureLoadFile("texture/select.png");
}


void DrawSelectButton() {
	if (buttons.empty()) {
		return;
	}
	//ボタンの描画
	for (auto itr = buttons.begin(); itr != buttons.end(); itr++) {
		if (std::distance(buttons.begin(), itr)==selectedIndex) {
			if (isPressed) {
				DrawSprite(itr->pressedTextureId, itr->pos, 100, itr->size);
			}
			else {
				DrawSprite(itr->textureId, itr->pos, 100, itr->size);
			}

			DrawSprite(selectFrameTextureId, itr->pos, 100, itr->size);
		}
		else {
			DrawSprite(itr->textureId, itr->pos, 100, itr->size);
		}
	}



}

void UninitSelectButton() {
	ReleaseTexture(selectFrameTextureId);
	for (const auto& b : buttons) {
		ReleaseTexture(b.textureId);
		ReleaseTexture(b.pressedTextureId);
	}
	buttons.clear();
}

void AddButton(Button& button) {
	buttons.push_back(button);
}

void ForwardSelectButton() {
	if (buttons.size()-1 <= selectedIndex) {
		return;
	}
	selectedIndex++;
	isPressed = false;
}

void BackSelectButton() {
	if (selectedIndex <= 0) {
		return;
	}
	selectedIndex--;
	isPressed = false;
}

void TriggerSelectButton() {
	isPressed = true;
	if (buttons[selectedIndex].triggeredCallback != NULL) {
		buttons[selectedIndex].triggeredCallback();
	}
}

void ReleaseSelectButton() {
	if (isPressed) {
		isPressed = false;
		if (buttons[selectedIndex].releasedCallback != NULL) {
			buttons[selectedIndex].releasedCallback();
		}
	}
}

void PressSelectButton() {
	if (buttons[selectedIndex].pressedCallback != NULL) {
		buttons[selectedIndex].pressedCallback();
	}
}
