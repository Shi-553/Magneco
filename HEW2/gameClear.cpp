#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "selectButton.h"

#define GAME_START_BUTTON_WIDTH 256
#define GAME_START_BUTTON_HEIGHT 128

int gameClearTexture;

Button startReturnButton, nextStageButton;

void InitGameClear()
{
	InitSelectButton();

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_START_BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - GAME_START_BUTTON_HEIGHT / 2);

	startReturnButton.pos = buttonCenter;
	startReturnButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	startReturnButton.frameColor = D3DCOLOR_RGBA(0, 0, 0, 255);
	startReturnButton.textureId = ReserveTextureLoadFile("texture/startButton.png");

	startReturnButton.pressedCallback = []() {
		GoNextScene(GameStartScene);
	};

	nextStageButton.pos = buttonCenter + D3DXVECTOR2(0, 200);
	nextStageButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	nextStageButton.frameColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	nextStageButton.textureId = ReserveTextureLoadFile("texture/endButton.png");

	nextStageButton.pressedCallback = []() {
		GoNextScene(GameScene);
	};

	AddButton(&startReturnButton);
	AddButton(&nextStageButton);

	gameClearTexture = ReserveTextureLoadFile("texture/”wŒi‚P.png");
	LoadTexture();
}

void UninitGameClear()
{
	UninitSelectButton();

	ReleaseTexture(gameClearTexture);
	ReleaseTexture(startReturnButton.textureId);
	ReleaseTexture(nextStageButton.textureId);
}

void DrawGameClear()
{
	DrawSprite(gameClearTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSelectButton();
}

void UpdateGameClear()
{
	if (ReleaseInputLogger(MYVK_ENTER)) {
		EnterSelectButton();
	}
	if (TriggerInputLogger(MYVK_UP)) {
		BackSelectButton();
	}
	if (TriggerInputLogger(MYVK_DOWN)) {
		ForwardSelectButton();
	}
}


