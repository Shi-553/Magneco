
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

static int quitTexture;
static int quitPressedTexture;
static int retryTexture;
static int retryPressedTexture;

static Button retryButton, returnTitleButton;

void InitGameOver() {
	InitSelectButton();
	quitTexture = ReserveTextureLoadFile("texture/quit.png");
	quitPressedTexture = ReserveTextureLoadFile("texture/quit_pressed.png");
	retryTexture = ReserveTextureLoadFile("texture/retry.png");
	retryPressedTexture = ReserveTextureLoadFile("texture/retry_pressed.png");


	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_OVER_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_OVER_BUTTON_HEIGHT / 2) + 40);

	retryButton.pos = buttonCenter;
	retryButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	retryButton.textureId = retryTexture;

	retryButton.triggeredCallback = []() {
		retryButton.textureId = retryPressedTexture;
	};
	retryButton.releasedCallback = []() {
		GoNextScene(GameScene);
	};

	returnTitleButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_OVER_BUTTON_HEIGHT + 32);
	returnTitleButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	returnTitleButton.textureId = quitTexture;

	returnTitleButton.triggeredCallback = []() {
		returnTitleButton.textureId = quitPressedTexture;
	};
	returnTitleButton.releasedCallback = []() {
		GoNextScene(GameStartScene);
	};

	AddButton(&retryButton);
	AddButton(&returnTitleButton);

	backgroundTexture = ReserveTextureLoadFile("texture/îwåi2.jpg");

	gameOverTextTexture = ReserveTextureLoadFile("texture/gameover.png");

	LoadTexture();
}

void UninitGameOver() {
	UninitSelectButton();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameOverTextTexture);
	ReleaseTexture(quitTexture);
	ReleaseTexture(quitPressedTexture);
	ReleaseTexture(retryTexture);
	ReleaseTexture(retryPressedTexture);

}
void DrawGameOver() {
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(gameOverTextTexture, { 200,64 }, 10, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT }, { 0,0 }, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT });
	DrawSelectButton();
}

static bool isChange = false;
void UpdateGameOver()
{
	if (TriggerInputLogger(MYVK_ENTER)) {
		TriggerSelectButton();
		isChange = false;
	}
	if (!isChange && ReleaseInputLogger(MYVK_ENTER)) {
		ReleaseSelectButton();
	}
	if (TriggerInputLogger(MYVK_UP)) {
		BackSelectButton();
		returnTitleButton.textureId = quitTexture;
		retryButton.textureId = retryTexture;
		isChange = true;
	}
	if (TriggerInputLogger(MYVK_DOWN)) {
		ForwardSelectButton();
		returnTitleButton.textureId = quitTexture;
		retryButton.textureId = retryTexture;
		isChange = true;
	}
}