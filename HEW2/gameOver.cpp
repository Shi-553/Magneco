
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


void InitGameOver() {
	InitSelectButton();

Button retryButton, returnTitleButton;

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_OVER_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_OVER_BUTTON_HEIGHT / 2) + 40);

	retryButton.pos = buttonCenter;
	retryButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	retryButton.textureId = ReserveTextureLoadFile("texture/retry.png");
	retryButton.pressedTextureId = ReserveTextureLoadFile("texture/retry_pressed.png");

	retryButton.releasedCallback = []() {
		GoNextScene(GameScene);
	};

	returnTitleButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_OVER_BUTTON_HEIGHT + 32);
	returnTitleButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	returnTitleButton.textureId = ReserveTextureLoadFile("texture/quit.png");
	returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/quit_pressed.png");

	returnTitleButton.releasedCallback = []() {
		GoNextScene(GameStartScene);
	};

	AddButton(retryButton);
	AddButton(returnTitleButton);

	backgroundTexture = ReserveTextureLoadFile("texture/背景2.jpg");

	gameOverTextTexture = ReserveTextureLoadFile("texture/gameover.png");

	LoadTexture();
}

void UninitGameOver() {
	UninitSelectButton();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameOverTextTexture);

}
void DrawGameOver() {
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(gameOverTextTexture, { 200,64 }, 10, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT }, { 0,0 }, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT });
	DrawSelectButton();
}

void UpdateGameOver()
{
	if (TriggerInputLogger(MYVK_ENTER)) {
		TriggerSelectButton();
	}
	if ( ReleaseInputLogger(MYVK_ENTER)) {
		ReleaseSelectButton();
	}
	if (TriggerInputLogger(MYVK_UP)) {
		BackSelectButton();
	}
	if (TriggerInputLogger(MYVK_DOWN)) {
		ForwardSelectButton();
	}
}