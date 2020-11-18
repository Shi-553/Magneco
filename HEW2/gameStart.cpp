
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameStart.h"
#include "sceneManager.h"
#include "InputLogger.h"
#include "selectButton.h"

#define GAME_START_BUTTON_WIDTH 256
#define GAME_START_BUTTON_HEIGHT 128

#define TITLE_LOGO_WIDTH 640
#define TITLE_LOGO_HEIGHT 240

static int backGroundTexture;
static int titleTextTexture;

static Button startButton, endButton;

void InitGameStart() {
	InitSelectButton();

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_START_BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - GAME_START_BUTTON_HEIGHT / 2);

	startButton.pos = buttonCenter;
	startButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	startButton.frameColor = D3DCOLOR_RGBA(0,0,0,255);
	startButton.textureId = ReserveTextureLoadFile("texture/startButton.png");

	startButton.releasedCallback = []() {
		GoNextScene(GameScene);
	};

	endButton.pos = buttonCenter+ D3DXVECTOR2(0,200);
	endButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	endButton.frameColor = D3DCOLOR_RGBA(255,0,0, 255);
	endButton.textureId = ReserveTextureLoadFile("texture/endButton.png");

	endButton.releasedCallback = []() {
		PostQuitMessage(0);
	};

	AddButton(&startButton);
	AddButton(&endButton);

	backGroundTexture = ReserveTextureLoadFile("texture/îwåiÇP.png");

	titleTextTexture = ReserveTextureLoadFile("texture/title_640Å~240.png");

	LoadTexture();
}

void UninitGameStart() {
	UninitSelectButton();

	ReleaseTexture(backGroundTexture);
	ReleaseTexture(titleTextTexture);
	ReleaseTexture(startButton.textureId);
	ReleaseTexture(endButton.textureId);

}
void DrawGameStart() {
	DrawSprite(backGroundTexture, { 0,0 }, 10, {SCREEN_WIDTH,SCREEN_HEIGHT}, {0,0}, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(titleTextTexture, { 325,0 }, 10, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT }, { 0,0 }, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT });
	DrawSelectButton();
}

void UpdateGameStart() {
	if (ReleaseInputLogger(MYVK_ENTER)) {
		ReleaseSelectButton();
	}
	if (TriggerInputLogger(MYVK_UP)) {
		BackSelectButton();
	}
	if (TriggerInputLogger(MYVK_DOWN)) {
		ForwardSelectButton();
	}
}
