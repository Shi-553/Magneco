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

static Button startReturnButton, nextStageButton;

void InitGameClear()
{
	InitSelectButton();

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_CLEAR_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_CLEAR_BUTTON_HEIGHT / 2) + 40);

	startReturnButton.pos = buttonCenter;
	startReturnButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	startReturnButton.textureId = ReserveTextureLoadFile("texture/quit.png");

	startReturnButton.pressedCallback = []() {
		GoNextScene(GameStartScene);
	};

	nextStageButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 32);
	nextStageButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	nextStageButton.textureId = ReserveTextureLoadFile("texture/retry.png");

	nextStageButton.pressedCallback = []() {
		GoNextScene(GameScene);
	};

	AddButton(&startReturnButton);
	AddButton(&nextStageButton);

	backgroundTexture = ReserveTextureLoadFile("texture/”wŒi‚P.png");
	gameClearTexture = ReserveTextureLoadFile("texture/stageclear.png");

	LoadTexture();
}

void UninitGameClear()
{
	UninitSelectButton();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameClearTexture);
	ReleaseTexture(startReturnButton.textureId);
	ReleaseTexture(nextStageButton.textureId);
}

void DrawGameClear()
{
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(gameClearTexture, { 200,64 }, 10, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT }, { 0,0 }, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT });
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


