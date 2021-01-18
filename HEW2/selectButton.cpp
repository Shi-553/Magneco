#include "selectButton.h"
#include "sprite.h"
#include <vector>



static std::vector<Button> buttons;
static int selectedIndex;


static bool isPressedEnter = false;
static bool isPressedForward = false;
static bool isPressedBack = false;

static VirtualKey gEnterKey, gForwardKey, gBackKey;


static int selectFrameTextureId = TEXTURE_INVALID_ID;

static D3DXVECTOR2 forwardPos, backPos;
static D3DXVECTOR2 forwardSize, backSize;

static int forwardTextureId = TEXTURE_INVALID_ID;
static int forwardPressedTextureId = TEXTURE_INVALID_ID;
static int forwardCantSelectTextureId = TEXTURE_INVALID_ID;

static int backTextureId = TEXTURE_INVALID_ID;
static int backPressedTextureId = TEXTURE_INVALID_ID;
static int backCantSelectTextureId = TEXTURE_INVALID_ID;


void AllReleasePressedFlag();


void InitSelectButton() {
	UninitSelectButton();

	selectedIndex = 0;

	SetSelectButtonKey(MYVK_ENTER, MYVK_DOWN, MYVK_UP);
	AllReleasePressedFlag();
}


void DrawSelectButton() {
	if (buttons.empty()) {
		return;
	}
	//ボタンの描画
	for (auto itr = buttons.begin(); itr != buttons.end(); itr++) {
		bool isSelected = std::distance(buttons.begin(), itr) == selectedIndex;

		if (itr->size == D3DXVECTOR2(0, 0)) {
			if (auto s = GetTextureSize(itr->textureId)) {
				itr->size = *s;
			}
		}

		if (isSelected && isPressedEnter) {
			DrawSprite(itr->pressedTextureId, itr->pos, 100, itr->size);
		}
		else {
			DrawSprite(itr->textureId, itr->pos, 100, itr->size);
		}

		if (isSelected) {
			DrawSprite(selectFrameTextureId, itr->pos, 100, itr->size);
		}
	}

	if (forwardPressedTextureId != TEXTURE_INVALID_ID) {
		if (forwardSize == D3DXVECTOR2(0, 0)) {
			if (auto s = GetTextureSize(forwardTextureId)) {
				forwardSize = *s;
			}
		}
		if (buttons.size() - 1 > selectedIndex) {

			if (isPressedForward) {
				DrawSprite(forwardPressedTextureId, forwardPos, 100, forwardSize);
			}
			else {
				DrawSprite(forwardTextureId, forwardPos, 100, forwardSize);
			}
		}
		else {
			DrawSprite(forwardCantSelectTextureId, forwardPos, 100, forwardSize);
		}
	}

	if (backTextureId != TEXTURE_INVALID_ID) {
		if (backSize == D3DXVECTOR2(0, 0)) {
			if (auto s = GetTextureSize(backTextureId)) {
				backSize = *s;
			}
		}
		if (selectedIndex > 0) {

			if (isPressedBack) {
				DrawSprite(backPressedTextureId, backPos, 100, backSize);
			}
			else {
				DrawSprite(backTextureId, backPos, 100, backSize);
			}
		}
		else {

			DrawSprite(backCantSelectTextureId, backPos, 100, backSize);
		}
	}


}
void UpdateSelectButton() {
	if (TriggerInputLogger(gEnterKey)) {
		AllReleasePressedFlag();
		isPressedEnter = true;
		if (buttons[selectedIndex].triggeredCallback != NULL) {
			buttons[selectedIndex].triggeredCallback();
		}
	}
	if (PressInputLogger(gEnterKey)) {
		if (buttons[selectedIndex].pressedCallback != NULL) {
			buttons[selectedIndex].pressedCallback();
		}
	}
	if (ReleaseInputLogger(gEnterKey)) {
		if (isPressedEnter) {
			AllReleasePressedFlag();
			if (buttons[selectedIndex].releasedCallback != NULL) {
				buttons[selectedIndex].releasedCallback();
			}
		}
	}
	if (TriggerInputLogger(gForwardKey)) {
		if (buttons.size() - 1 > selectedIndex) {
			selectedIndex++;

			AllReleasePressedFlag();
			isPressedForward = true;
		}
	}
	if (ReleaseInputLogger(gForwardKey)) {
		AllReleasePressedFlag();
	}
	if (TriggerInputLogger(gBackKey)) {
		if (selectedIndex > 0) {
			selectedIndex--;
			AllReleasePressedFlag();
			isPressedBack = true;
		}
	}
	if (ReleaseInputLogger(gBackKey)) {
		AllReleasePressedFlag();
	}
}

void UninitSelectButton() {
	ReleaseTexture(selectFrameTextureId);
	ReleaseTexture(forwardTextureId);
	ReleaseTexture(forwardPressedTextureId);
	ReleaseTexture(backTextureId);
	ReleaseTexture(backPressedTextureId);

	selectFrameTextureId = TEXTURE_INVALID_ID;
	forwardTextureId = TEXTURE_INVALID_ID;
	forwardPressedTextureId = TEXTURE_INVALID_ID;
	backTextureId = TEXTURE_INVALID_ID;
	backPressedTextureId = TEXTURE_INVALID_ID;

	for (const auto& b : buttons) {
		ReleaseTexture(b.textureId);
		ReleaseTexture(b.pressedTextureId);
	}
	buttons.clear();
}

void AllReleasePressedFlag() {
	isPressedEnter = false;
	isPressedForward = false;
	isPressedBack = false;
}

void AddSelectButton(Button& button) {

	buttons.push_back(button);
}
void SetSelectButtonKey(VirtualKey enterKey, VirtualKey forwardKey, VirtualKey backKey) {
	gEnterKey = enterKey;
	gForwardKey = forwardKey;
	gBackKey = backKey;
}

void SetSelectButtonFrame(int frame) {
	selectFrameTextureId = frame;
}
void SetSelectButtonForward(int forward, int forwardPressed, int forwardCantSelect, D3DXVECTOR2 pos, D3DXVECTOR2 size) {
	forwardTextureId = forward;
	forwardPressedTextureId = forwardPressed;
	forwardCantSelectTextureId = forwardCantSelect;
	forwardPos = pos;
	forwardSize = size;
}
void SetSelectButtonBack(int back, int backPressed, int backCantSelect, D3DXVECTOR2 pos, D3DXVECTOR2 size) {
	backTextureId = back;
	backPressedTextureId = backPressed;
	backCantSelectTextureId = backCantSelect;
	backPos = pos;
	backSize = size;
}

int GetSelectButtonIndex() {
	return selectedIndex;
}