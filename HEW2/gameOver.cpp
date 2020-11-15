
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameOver.h"
#include "sceneManager.h"
#include "InputLogger.h"
#include "selectButton.h"

#define GAME_OVER_BUTTON_WIDTH 256
#define GAME_OVER_BUTTON_HEIGHT 128

#define GAME_OVER_LOGO_WIDTH 640
#define GAME_OVER_LOGO_HEIGHT 240

static int backgroundTexture;
static int gameOverTextTexture;

static Button restartButton, backtitleButton;

void InitGameOver() {
	InitSelectButton();

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_OVER_BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 - GAME_OVER_BUTTON_HEIGHT / 2);

	restartButton.pos = buttonCenter;
	restartButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	restartButton.frameColor = D3DCOLOR_RGBA(0, 0, 0, 255);
	restartButton.textureId = ReserveTextureLoadFile("texture/restartButton.png");

	restartButton.pressedCallback = []() {
		GoNextScene(GameScene);
	};

	backtitleButton.pos = buttonCenter + D3DXVECTOR2(0, 200);
	backtitleButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	backtitleButton.frameColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	backtitleButton.textureId = ReserveTextureLoadFile("texture/backtitleButton.png");

	backtitleButton.pressedCallback = []() {
		GoNextScene(GameStartScene);
	};

	AddButton(&restartButton);
	AddButton(&backtitleButton);

	backgroundTexture = ReserveTextureLoadFile("texture/îwåiÇP.png");

	gameOverTextTexture = ReserveTextureLoadFile("texture/gameover_640Å~240.png");

	LoadTexture();
}

void UninitGameOver() {
	UninitSelectButton();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameOverTextTexture);
	ReleaseTexture(restartButton.textureId);
	ReleaseTexture(backtitleButton.textureId);

}
void DrawGameOver() {
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(gameOverTextTexture, { 325,0 }, 10, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT }, { 0,0 }, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT });
	DrawSelectButton();
}

void UpdateGameOver() {
	if (TriggerInputLogger(MYVK_ENTER)) {
		EnterSelectButton();
	}
	if (TriggerInputLogger(MYVK_UP)) {
		BackSelectButton();
	}
	if (TriggerInputLogger(MYVK_DOWN)) {
		ForwardSelectButton();
	}
}
