#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "selectButton.h"


#define GAME_CLEAR_BUTTON_WIDTH 330
#define GAME_CLEAR_BUTTON_HEIGHT 90

#define GAME_CLEAR_LOGO_WIDTH 896
#define GAME_CLEAR_LOGO_HEIGHT 224

static int backgroundTexture;
static int gameClearTexture;

void InitGameClear()
{
	InitSelectButton();

 Button returnTitleButton, retryButton;


	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_CLEAR_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_CLEAR_BUTTON_HEIGHT / 2) + 40);

	returnTitleButton.pos = buttonCenter;
	returnTitleButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	returnTitleButton.textureId = ReserveTextureLoadFile("texture/quit.png");
	returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/quit_pressed.png");

	returnTitleButton.releasedCallback = []() {
		GoNextScene(GameStartScene);
	};

	retryButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 32);
	retryButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	retryButton.textureId = ReserveTextureLoadFile("texture/retry.png");
	retryButton.pressedTextureId = ReserveTextureLoadFile("texture/retry_pressed.png");


	retryButton.releasedCallback = []() {
		GoNextScene(GameScene);
	};

	AddButton(returnTitleButton);
	AddButton(retryButton);

	backgroundTexture = ReserveTextureLoadFile("texture/背景１.png");
	gameClearTexture = ReserveTextureLoadFile("texture/stageclear.png");

	LoadTexture();
}

void UninitGameClear()
{
	UninitSelectButton();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameClearTexture);
}

void DrawGameClear()
{
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(gameClearTexture, { 200,64 }, 10, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT }, { 0,0 }, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT });
	DrawSelectButton();
}

void UpdateGameClear()
{
	if (TriggerInputLogger(MYVK_ENTER)) {
		TriggerSelectButton();
	}
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


