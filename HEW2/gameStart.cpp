
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameStart.h"
#include "sceneManager.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "stageSelect.h"

#define GAME_START_BUTTON_WIDTH 330
#define GAME_START_BUTTON_HEIGHT 90

#define TITLE_LOGO_WIDTH 640
#define TITLE_LOGO_HEIGHT 240

#define BUTTON_DESCRIPTION_LOGO_WIDTH 372
#define BUTTON_DESCRIPTION_LOGO_HEIGHT 49

static int backGroundTexture;
static int titleTextTexture;
static int buttonDescriptionTexture;

static SelectButton gameStartSelect;
void InitGameStart() {
	gameStartSelect.Init();


	Button startButton, tutorialButton, endButton;

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_START_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_START_BUTTON_HEIGHT / 2) + 40);

	startButton.pos = buttonCenter;
	startButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	startButton.textureId = ReserveTextureLoadFile("texture/ui/start.png");
	startButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/start_pressed.png");

	startButton.releasedCallback = []() {
		SetStageFolder("stage/main");
		GoNextScene(StageSelect);
	};


	tutorialButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_START_BUTTON_HEIGHT + 16);
	tutorialButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	tutorialButton.textureId = ReserveTextureLoadFile("texture/ui/tutorial.png");
	tutorialButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/tutorial_pressed.png");


	tutorialButton.releasedCallback = []() {
		SetStageFolder("stage/tutorial");
		GoNextScene(StageSelect);
	};

	endButton.pos = tutorialButton.pos + D3DXVECTOR2(0, GAME_START_BUTTON_HEIGHT + 16);
	endButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	endButton.textureId = ReserveTextureLoadFile("texture/ui/end.png");
	endButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/end_pressed.png");


	endButton.releasedCallback = []() {
		PostQuitMessage(0);
	};

	gameStartSelect.Add(startButton);
	gameStartSelect.Add(tutorialButton);
	gameStartSelect.Add(endButton);

	backGroundTexture = ReserveTextureLoadFile("texture/background/背景１.png");

	titleTextTexture = ReserveTextureLoadFile("texture/ui/title_640×240.png");

	buttonDescriptionTexture = ReserveTextureLoadFile("texture/ui/tips.png");

	gameStartSelect.SetFrame(ReserveTextureLoadFile("texture/ui/select.png"));

	LoadTexture();
}

void UninitGameStart() {
	gameStartSelect.Uninit();

	ReleaseTexture(backGroundTexture);
	ReleaseTexture(titleTextTexture);
	ReleaseTexture(buttonDescriptionTexture);

}
void DrawGameStart() {
	DrawSprite(backGroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(titleTextTexture, { 320,64 }, 10, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT }, { 0,0 }, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT });
	DrawSprite(buttonDescriptionTexture, { 32,640 }, 10, { BUTTON_DESCRIPTION_LOGO_WIDTH,BUTTON_DESCRIPTION_LOGO_HEIGHT }, { 0,0 }, { BUTTON_DESCRIPTION_LOGO_WIDTH,BUTTON_DESCRIPTION_LOGO_HEIGHT });
	gameStartSelect.Draw();
}

void UpdateGameStart() {
	gameStartSelect.Update();
}
