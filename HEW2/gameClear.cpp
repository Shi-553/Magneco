#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "selectButton.h"


#define GAME_CLEAR_BUTTON_WIDTH 330
#define GAME_CLEAR_BUTTON_HEIGHT 90

#define GAME_CLEAR_LOGO_WIDTH 1024
#define GAME_CLEAR_LOGO_HEIGHT 256

static int backgroundTexture;
static int gameClearTexture;

static int quitTexture;
static int quitPressedTexture;
static int retryTexture;
static int retryPressedTexture;

static int frame = 0;

static Button returnTitleButton, retryButton;

void InitGameClear()
{
	InitSelectButton();
	quitTexture= ReserveTextureLoadFile("texture/quit.png");
	quitPressedTexture= ReserveTextureLoadFile("texture/quit_pressed.png");
	retryTexture= ReserveTextureLoadFile("texture/retry.png");
	retryPressedTexture= ReserveTextureLoadFile("texture/retry_pressed.png");

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_CLEAR_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_CLEAR_BUTTON_HEIGHT / 2) + 40);

	returnTitleButton.pos = buttonCenter;
	returnTitleButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	returnTitleButton.textureId = quitTexture;

	returnTitleButton.triggeredCallback = []() {
		returnTitleButton.textureId = quitPressedTexture;
	};
	returnTitleButton.releasedCallback = []() {
		GoNextScene(GameStartScene);
	};

	retryButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 32);
	retryButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	retryButton.textureId = retryTexture;

	retryButton.triggeredCallback = []() {
		retryButton.textureId = retryPressedTexture;
	};

	retryButton.releasedCallback = []() {
		GoNextScene(GameScene);
	};

	AddButton(&returnTitleButton);
	AddButton(&retryButton);

	backgroundTexture = ReserveTextureLoadFile("texture/background/背景１.png");
	gameClearTexture = ReserveTextureLoadFile("texture/stageclear_1024×256.png");

	LoadTexture();
}

void UninitGameClear()
{
	UninitSelectButton();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameClearTexture);
	ReleaseTexture(quitTexture);
	ReleaseTexture(quitPressedTexture);
	ReleaseTexture(retryTexture);
	ReleaseTexture(retryPressedTexture);
}

void DrawGameClear()
{
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });

		auto tPos = D3DXVECTOR2(
			0,
			GAME_CLEAR_LOGO_HEIGHT * (frame / 4 % 4)
		);

	DrawSprite(gameClearTexture, { 200,64 }, 10, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT }, tPos, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT });
	DrawSelectButton();
}

static bool isChange = false;

void UpdateGameClear()
{
	if (TriggerInputLogger(MYVK_ENTER)) {
		TriggerSelectButton();
		isChange = false;
	}
	if (!isChange&&ReleaseInputLogger(MYVK_ENTER)) {
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

	frame++;
}


