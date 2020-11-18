
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameStart.h"
#include "sceneManager.h"
#include "InputLogger.h"
#include "selectButton.h"

#define GAME_START_BUTTON_WIDTH 330
#define GAME_START_BUTTON_HEIGHT 90

#define TITLE_LOGO_WIDTH 640
#define TITLE_LOGO_HEIGHT 240

#define BUTTON_DESCRIPTION_LOGO_WIDTH 372
#define BUTTON_DESCRIPTION_LOGO_HEIGHT 49

static int backGroundTexture;
static int titleTextTexture;
static int buttonDescriptionTexture;

static Button startButton, endButton;

void InitGameStart() {
	InitSelectButton();

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_START_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_START_BUTTON_HEIGHT / 2) + 40);

	startButton.pos = buttonCenter;
	startButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	startButton.textureId = ReserveTextureLoadFile("texture/start.png");

	startButton.pressedCallback = []() {
		GoNextScene(GameScene);
	};

	endButton.pos = buttonCenter+ D3DXVECTOR2(0, GAME_START_BUTTON_HEIGHT + 32);
	endButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	endButton.textureId = ReserveTextureLoadFile("texture/end.png");

	endButton.pressedCallback = []() {
		PostQuitMessage(0);
	};

	AddButton(&startButton);
	AddButton(&endButton);

	backGroundTexture = ReserveTextureLoadFile("texture/îwåiÇP.png");

	titleTextTexture = ReserveTextureLoadFile("texture/title_640Å~240.png");

	buttonDescriptionTexture = ReserveTextureLoadFile("texture/tips.png");

	LoadTexture();
}

void UninitGameStart() {
	UninitSelectButton();

	ReleaseTexture(backGroundTexture);
	ReleaseTexture(titleTextTexture);
	ReleaseTexture(buttonDescriptionTexture);
	ReleaseTexture(startButton.textureId);
	ReleaseTexture(endButton.textureId);

}
void DrawGameStart() {
	DrawSprite(backGroundTexture, { 0,0 }, 10, {SCREEN_WIDTH,SCREEN_HEIGHT}, {0,0}, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(titleTextTexture, { 320,64 }, 10, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT }, { 0,0 }, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT });
	DrawSprite(buttonDescriptionTexture, { 32,640 }, 10, { BUTTON_DESCRIPTION_LOGO_WIDTH,BUTTON_DESCRIPTION_LOGO_HEIGHT }, { 0,0 }, { BUTTON_DESCRIPTION_LOGO_WIDTH,BUTTON_DESCRIPTION_LOGO_HEIGHT });
	DrawSelectButton();
}

void UpdateGameStart() {
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
