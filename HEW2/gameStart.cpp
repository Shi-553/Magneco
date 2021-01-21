
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameStart.h"
#include "sceneManager.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "sound.h"

#define GAME_START_BUTTON_WIDTH 330
#define GAME_START_BUTTON_HEIGHT 90

#define TITLE_LOGO_WIDTH 640
#define TITLE_LOGO_HEIGHT 240

#define BUTTON_DESCRIPTION_LOGO_WIDTH 372
#define BUTTON_DESCRIPTION_LOGO_HEIGHT 49

static int backGroundTexture;
static int titleTextTexture;
static int buttonDescriptionTexture;


void InitGameStart() {
	InitSelectButton();
	StopSound();
	PlaySound(SOUND_LABEL_BGM001);


	Button startButton, tutorialButton, endButton;

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_START_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_START_BUTTON_HEIGHT / 2) + 40);

	startButton.pos = buttonCenter;
	startButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	startButton.textureId = ReserveTextureLoadFile("texture/start.png");
	startButton.pressedTextureId = ReserveTextureLoadFile("texture/start_pressed.png");

	startButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		GoNextScene(StageSelect);
	};


	tutorialButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_START_BUTTON_HEIGHT + 16);
	tutorialButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	tutorialButton.textureId = ReserveTextureLoadFile("texture/tutorial.png");
	tutorialButton.pressedTextureId = ReserveTextureLoadFile("texture/tutorial_pressed.png");


	tutorialButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		GoNextScene(TutorialScene);
	};

	endButton.pos = tutorialButton.pos + D3DXVECTOR2(0, GAME_START_BUTTON_HEIGHT + 16);
	endButton.size = D3DXVECTOR2(GAME_START_BUTTON_WIDTH, GAME_START_BUTTON_HEIGHT);
	endButton.textureId = ReserveTextureLoadFile("texture/end.png");
	endButton.pressedTextureId = ReserveTextureLoadFile("texture/end_pressed.png");


	endButton.releasedCallback = []() {
		PostQuitMessage(0);
	};

	AddSelectButton(startButton);
	AddSelectButton(tutorialButton);
	AddSelectButton(endButton);

	backGroundTexture = ReserveTextureLoadFile("texture/背景１.png");

	titleTextTexture = ReserveTextureLoadFile("texture/title_640×240.png");

	buttonDescriptionTexture = ReserveTextureLoadFile("texture/tips.png");

	SetSelectButtonFrame(ReserveTextureLoadFile("texture/select.png"));

	LoadTexture();
}

void UninitGameStart() {
	UninitSelectButton();

	ReleaseTexture(backGroundTexture);
	ReleaseTexture(titleTextTexture);
	ReleaseTexture(buttonDescriptionTexture);

}
void DrawGameStart() {
	DrawSprite(backGroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(titleTextTexture, { 320,64 }, 10, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT }, { 0,0 }, { TITLE_LOGO_WIDTH,TITLE_LOGO_HEIGHT });
	DrawSprite(buttonDescriptionTexture, { 32,640 }, 10, { BUTTON_DESCRIPTION_LOGO_WIDTH,BUTTON_DESCRIPTION_LOGO_HEIGHT }, { 0,0 }, { BUTTON_DESCRIPTION_LOGO_WIDTH,BUTTON_DESCRIPTION_LOGO_HEIGHT });
	DrawSelectButton();
}

void UpdateGameStart() {
	UpdateSelectButton();
}
