﻿#include "pauseMenu.h"
#include <Windows.h>
#include "texture.h"
#include "sprite.h"
#include "config.h"
#include "time.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "sceneManager.h"
#include "sound.h"
#include <vector>
#include <map>
using namespace std;

#define PAUSE_MENU_BUTTON_WIDTH 330
#define PAUSE_MENU_BUTTON_HEIGHT 90

static int backgroundTextureId = TEXTURE_INVALID_ID;
static int pauseTextureId = TEXTURE_INVALID_ID;
static bool isPause = false;
static SelectButton pauseSelect;


std::map < int,SwitchPauseEvent> switchPauseEvent;

void InitPauseMenu() {
	isPause = false;
	SetTimeScale(1);

	backgroundTextureId = ReserveTextureLoadFile("texture/background/背景3.jpg");
	pauseTextureId = ReserveTextureLoadFile("texture/ui/pause.png");

	pauseSelect.Init();
	vector<Button> buttons;

	Button startButton;

	startButton.textureId = ReserveTextureLoadFile("texture/ui/tudukeru.png");
	startButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/tudukeru_pressed.png");
	startButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		isPause = false;
		SetTimeScale(1);
	};
	buttons.push_back(startButton);

	if (GetCurrentScene() == GameScene) {
		Button retryButton;
		retryButton.textureId = ReserveTextureLoadFile("texture/ui/retry.png");
		retryButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/retry_pressed.png");
		retryButton.releasedCallback = []() {
			PlaySound(SOUND_LABEL_SE_DECITION);
			GoNextScene(GameScene, FADE_OUT, true);
			isPause = false;
			SetTimeScale(1);
		};
		buttons.push_back(retryButton);

		Button stageSelectButton;
		stageSelectButton.textureId = ReserveTextureLoadFile("texture/ui/stageselect.png");
		stageSelectButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/stageselect_pressed.png");
		stageSelectButton.releasedCallback = []() {
			PlaySound(SOUND_LABEL_SE_DECITION);
			GoNextScene(StageSelect);
			isPause = false;
			SetTimeScale(1);
		};
		buttons.push_back(stageSelectButton);

	}
	else {
		if (GetCurrentScene() !=GameStartScene) {
			Button returnTitleButton;
			returnTitleButton.textureId = ReserveTextureLoadFile("texture/ui/quit.png");
			returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/quit_pressed.png");

			returnTitleButton.releasedCallback = []() {
				PlaySound(SOUND_LABEL_SE_DECITION);
				GoNextScene(GameStartScene);
				isPause = false;
				SetTimeScale(1);
			};
		buttons.push_back(returnTitleButton);
		}
		Button endButton;
		endButton.textureId = ReserveTextureLoadFile("texture/ui/end.png");
		endButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/end_pressed.png");

		endButton.releasedCallback = []() {
			PostQuitMessage(0);
			isPause = false;
			SetTimeScale(1);
		};
		buttons.push_back(endButton);
	}

	for (auto itr = buttons.begin(); itr != buttons.end(); itr++) {
		auto buttonCenter = D3DXVECTOR2(475,314+90);

		buttonCenter.y += (std::distance(buttons.begin(), itr) -( buttons.size()-1)/2.0f)*( PAUSE_MENU_BUTTON_HEIGHT+20);

		itr->pos = buttonCenter;
		itr->size = D3DXVECTOR2(PAUSE_MENU_BUTTON_WIDTH, PAUSE_MENU_BUTTON_HEIGHT);

		pauseSelect.Add(*itr);
	}
	pauseSelect.SetFrame(ReserveTextureLoadFile("texture/ui/select.png"));

	LoadTexture();
}

void DrawPauseMenu() {
	if (!isPause) {
		return;
	}
	DrawSprite(backgroundTextureId, D3DXVECTOR2(0, 0), 10, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	DrawSprite(pauseTextureId, D3DXVECTOR2(166, 62), 10);


	pauseSelect.DrawMain();
	pauseSelect.DrawLR();
}


void UninitPauseMenu() {
	ReleaseTexture(backgroundTextureId);
	ReleaseTexture(pauseTextureId);

	pauseSelect.Uninit();

}

void UpdatePauseMenu() {

	if (TriggerInputLogger(MYVK_ESC)|| (isPause&&TriggerInputLogger(MYVK_CANCEL))) {
		isPause = !isPause;

		for (auto& e : switchPauseEvent) {
			e.second(isPause);
		}

		if (isPause) {
			PlaySound(SOUND_LABEL_SE_PAUSE);
			pauseSelect.SetIndex(0);
			SetTimeScale(0);
		}
		else {
			SetTimeScale(1);

		}
	}

	if (!isPause) {
		return;
	}

	pauseSelect.Update();
}


bool GetIsPause() {
	return isPause;
}

int AddSwitchPauseEvent(const SwitchPauseEvent& e) {
	auto s = switchPauseEvent.size();
	switchPauseEvent.emplace(s,e);
	return s;
}
void RemoveSwitchPauseEvent(int i) {
	switchPauseEvent.erase(i);
}