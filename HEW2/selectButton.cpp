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
	   
static int backTextureId = TEXTURE_INVALID_ID;
static int backPressedTextureId = TEXTURE_INVALID_ID;


void AllReleasePressedFlag();


void InitSelectButton() {
	UninitSelectButton();

	selectedIndex = 0;

	SetSelectButtonKey(MYVK_ENTER,MYVK_DOWN, MYVK_UP);
	AllReleasePressedFlag();
}


void DrawSelectButton() {
	if (buttons.empty()) {
		return;
	}
	//ボタンの描画
	for (auto itr = buttons.begin(); itr != buttons.end(); itr++) {
		bool isSelected = std::distance(buttons.begin(), itr) == selectedIndex;

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

	if (isPressedForward) {
		DrawSprite(forwardPressedTextureId, forwardPos, 100, forwardSize);
	}
	else {
		DrawSprite(forwardTextureId, forwardPos, 100, forwardSize);
	}
	if (isPressedBack) {
		DrawSprite(backPressedTextureId, backPos, 100, backSize);
	}
	else {
		DrawSprite(backTextureId, backPos, 100, backSize);
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
void SetSelectButtonForward(int forward, int forwardPressed, D3DXVECTOR2 pos, D3DXVECTOR2 size) {
	forwardTextureId = forward;
	forwardPressedTextureId = forwardPressed;
	forwardPos = pos;
	forwardSize = size;
}
void SetSelectButtonBack(int back, int backPressed, D3DXVECTOR2 pos, D3DXVECTOR2 size) {
	backTextureId = back;
	backPressedTextureId = backPressed;
	backPos = pos;
	backSize = size;
}