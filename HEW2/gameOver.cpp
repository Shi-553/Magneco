
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameOver.h"
#include "sceneManager.h"
#include "InputLogger.h"
#include "selectButton.h"

#define GAME_OVER_BUTTON_WIDTH 330
#define GAME_OVER_BUTTON_HEIGHT 90

#define GAME_OVER_LOGO_WIDTH 896
#define GAME_OVER_LOGO_HEIGHT 224

static int backgroundTexture;
static int gameOverTextTexture;

static Button restartButton, backtitleButton;

void InitGameOver() {
	InitSelectButton();

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_OVER_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_OVER_BUTTON_HEIGHT / 2) + 40);

	restartButton.pos = buttonCenter;
	restartButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	restartButton.textureId = ReserveTextureLoadFile("texture/retry.png");

	restartButton.releasedCallback = []() {
		GoNextScene(GameScene);
	};

	backtitleButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_OVER_BUTTON_HEIGHT + 32);
	backtitleButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	backtitleButton.textureId = ReserveTextureLoadFile("texture/quit.png");

	backtitleButton.releasedCallback = []() {
		GoNextScene(GameStartScene);
	};

	AddButton(&restartButton);
	AddButton(&backtitleButton);

	backgroundTexture = ReserveTextureLoadFile("texture/îwåi2.jpg");

	gameOverTextTexture = ReserveTextureLoadFile("texture/gameover.png");

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
	DrawSprite(gameOverTextTexture, { 200,64 }, 10, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT }, { 0,0 }, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT });
	DrawSelectButton();
}

void UpdateGameOver() {
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
