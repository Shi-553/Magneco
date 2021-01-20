#include "pauseMenu.h"
#include <Windows.h>
#include "texture.h"
#include "sprite.h"
#include "config.h"
#include "time.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "sceneManager.h"
#include <vector>
using namespace std;

#define PAUSE_MENU_BUTTON_WIDTH 330
#define PAUSE_MENU_BUTTON_HEIGHT 90

static int blackTextureId = TEXTURE_INVALID_ID;
static bool isPause = false;
static SelectButton pauseSelect;

void InitPauseMenu() {
	isPause = false;
	SetTimeScale(1);

	blackTextureId = ReserveTextureLoadFile("texture/fade.png");

	pauseSelect.Init();
	vector<Button> buttons;

	Button startButton;

	startButton.textureId = ReserveTextureLoadFile("texture/ui/start.png");
	startButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/start_pressed.png");
	startButton.releasedCallback = []() {
		isPause = false;
		SetTimeScale(1);
	};
	buttons.push_back(startButton);

	if (GetCurrentScene() == GameScene) {
		Button retryButton;
		retryButton.textureId = ReserveTextureLoadFile("texture/ui/retry.png");
		retryButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/retry_pressed.png");
		retryButton.releasedCallback = []() {
			GoNextScene(GameScene, FADE_OUT, true);
			isPause = false;
			SetTimeScale(1);
		};
		buttons.push_back(retryButton);

		Button returnTitleButton;
		returnTitleButton.textureId = ReserveTextureLoadFile("texture/ui/quit.png");
		returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/quit_pressed.png");

		returnTitleButton.releasedCallback = []() {
			GoNextScene(GameStartScene);
			isPause = false;
			SetTimeScale(1);
		};
		buttons.push_back(returnTitleButton);
	}
	else {
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
		auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - PAUSE_MENU_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - PAUSE_MENU_BUTTON_HEIGHT / 2));

		buttonCenter.y += (std::distance(buttons.begin(), itr) -( buttons.size()-1)/2.0f)*( PAUSE_MENU_BUTTON_HEIGHT+30);

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
	SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 200));
	DrawSprite(blackTextureId, D3DXVECTOR2(0, 0), 10, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	pauseSelect.Draw();
}


void UninitPauseMenu() {
	ReleaseTexture(blackTextureId);

	pauseSelect.Uninit();

}

void UpdatePauseMenu() {

	if (TriggerInputLogger(MYVK_ESC)) {
		isPause = !isPause;
		if (isPause) {
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