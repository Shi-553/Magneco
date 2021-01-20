#include "pauseMenu.h"
#include <Windows.h>
#include "texture.h"
#include "sprite.h"
#include "config.h"
#include "time.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "sceneManager.h"

#define PAUSE_MENU_BUTTON_WIDTH 330
#define PAUSE_MENU_BUTTON_HEIGHT 90

static int blackTextureId = TEXTURE_INVALID_ID;
static bool isPause = false;

void InitPauseMenu() {
	isPause = false;
	SetTimeScale(1);

	blackTextureId = ReserveTextureLoadFile("texture/fade.png");


	if (GetCurrentScene() != GameScene) {
		return;
	}

	InitSelectButton();

	Button startButton,returnTitleButton, retryButton;


	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - PAUSE_MENU_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - PAUSE_MENU_BUTTON_HEIGHT / 2) + 40);


	startButton.pos = buttonCenter - D3DXVECTOR2(0, PAUSE_MENU_BUTTON_HEIGHT + 32);
	startButton.size = D3DXVECTOR2(PAUSE_MENU_BUTTON_WIDTH, PAUSE_MENU_BUTTON_HEIGHT);
	startButton.textureId = ReserveTextureLoadFile("texture/ui/start.png");
	startButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/start_pressed.png");

	startButton.releasedCallback = []() {
		isPause = false;
		SetTimeScale(1);
	};

	returnTitleButton.pos = buttonCenter + D3DXVECTOR2(0, PAUSE_MENU_BUTTON_HEIGHT + 32);
	returnTitleButton.size = D3DXVECTOR2(PAUSE_MENU_BUTTON_WIDTH, PAUSE_MENU_BUTTON_HEIGHT);
	returnTitleButton.textureId = ReserveTextureLoadFile("texture/ui/quit.png");
	returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/quit_pressed.png");

	returnTitleButton.releasedCallback = []() {
		GoNextScene(GameStartScene);
		isPause = false;
		SetTimeScale(1);
	};

	retryButton.pos = buttonCenter;
	retryButton.size = D3DXVECTOR2(PAUSE_MENU_BUTTON_WIDTH, PAUSE_MENU_BUTTON_HEIGHT);
	retryButton.textureId = ReserveTextureLoadFile("texture/ui/retry.png");
	retryButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/retry_pressed.png");


	retryButton.releasedCallback = []() {
		GoNextScene(GameScene, FADE_OUT, true);
		isPause = false;
		SetTimeScale(1);
	};

	AddSelectButton(startButton);
	AddSelectButton(retryButton);
	AddSelectButton(returnTitleButton);

	SetSelectButtonFrame(ReserveTextureLoadFile("texture/ui/select.png"));

	LoadTexture();
}

void DrawPauseMenu() {
	if (!isPause) {
		return;
	}
	SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 150));
	DrawSprite(blackTextureId, D3DXVECTOR2(0, 0), 10, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	if (GetCurrentScene() != GameScene) {
		return;
	}
	DrawSelectButton();
}


void UninitPauseMenu() {
	ReleaseTexture(blackTextureId);

	if (GetCurrentScene() != GameScene) {
		return;
	}
	UninitSelectButton();

}

void UpdatePauseMenu() {

	if (isPause && GetCurrentScene() != GameScene) {
		if (MessageBox(NULL, "終了していい？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			PostQuitMessage(0);
			return;
		}
		isPause = false;
		SetTimeScale(1);
	}

	if (TriggerInputLogger(MYVK_ESC)) {
		isPause = !isPause;
		if (isPause) {
			DrawPauseMenu();

			SetTimeScale(0);
		}
		else {
			SetTimeScale(1);

		}
	}

	if (GetCurrentScene() != GameScene) {
		return;
	}
	if (!isPause) {
		return;
	}

	UpdateSelectButton();
}