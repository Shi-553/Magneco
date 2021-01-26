#include "selectButton.h"
#include "sprite.h"



void SelectButton::Init() {
	Uninit();

	selectedIndex = 0;

	SetKey(MYVK_ENTER, MYVK_DOWN, MYVK_UP);
	AllReleasePressedFlag();
}


void SelectButton::Draw() {
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
void SelectButton::Update() {
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
		selectedIndex++;
		if (buttons.size() <= selectedIndex) {
			selectedIndex = 0;
		}

		AllReleasePressedFlag();
		isPressedForward = true;
		if (buttons[selectedIndex].rightCallback != NULL) {
			buttons[selectedIndex].rightCallback();
		}
	}
	if (ReleaseInputLogger(gForwardKey)) {
		AllReleasePressedFlag();
	}
	if (TriggerInputLogger(gBackKey)) {
		selectedIndex--;
		if (selectedIndex <0) {
			selectedIndex=buttons.size() - 1;
		}
		AllReleasePressedFlag();
		isPressedBack = true;
		if (buttons[selectedIndex].leftCallback != NULL) {
			buttons[selectedIndex].leftCallback();
		}
	}
	if (ReleaseInputLogger(gBackKey)) {
		AllReleasePressedFlag();
	}
}

void SelectButton::Uninit() {
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

void SelectButton::AllReleasePressedFlag() {
	isPressedEnter = false;
	isPressedForward = false;
	isPressedBack = false;
}

void SelectButton::Add(Button& button) {

	buttons.push_back(button);
}
void SelectButton::SetKey(VirtualKey enterKey, VirtualKey forwardKey, VirtualKey backKey) {
	gEnterKey = enterKey;
	gForwardKey = forwardKey;
	gBackKey = backKey;
}

void SelectButton::SetFrame(int frame) {
	selectFrameTextureId = frame;
}
void SelectButton::SetForward(int forward, int forwardPressed, D3DXVECTOR2 pos, D3DXVECTOR2 size) {
	forwardTextureId = forward;
	forwardPressedTextureId = forwardPressed;
	forwardPos = pos;
	forwardSize = size;
}
void SelectButton::SetBack(int back, int backPressed, D3DXVECTOR2 pos, D3DXVECTOR2 size) {
	backTextureId = back;
	backPressedTextureId = backPressed;
	backPos = pos;
	backSize = size;
}

int SelectButton::GetIndex() {
	return selectedIndex;
}
void SelectButton::SetIndex(int index) {
	if (index < 0 || buttons.size() <= index) {
		return;
	}
	selectedIndex = index;
}