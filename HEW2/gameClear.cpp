#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "selectButton.h"
#include "sound.h"


#define GAME_CLEAR_BUTTON_WIDTH 330
#define GAME_CLEAR_BUTTON_HEIGHT 90

#define GAME_CLEAR_LOGO_WIDTH 1024
#define GAME_CLEAR_LOGO_HEIGHT 256

static int frame = 0;

static int backgroundTexture;
static int gameClearTexture;

static SelectButton gameClearSelect;

void InitGameClear()
{
	gameClearSelect.Init();
	StopSound();
	PlaySound(SOUND_LABEL_BGM004);


 Button returnTitleButton, stageSelectButton, retryButton;


	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_CLEAR_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_CLEAR_BUTTON_HEIGHT / 2) + 30);


	stageSelectButton.pos = buttonCenter;
	stageSelectButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	stageSelectButton.textureId = ReserveTextureLoadFile("texture/ui/stageselect.png");
	stageSelectButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/stageselect_pressed.png");


	stageSelectButton.releasedCallback = []() {
		StopSound();
		PlaySound(SOUND_LABEL_BGM004);
		GoNextScene(StageSelect);
	};
	returnTitleButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		GoNextScene(GameStartScene);
	};

	retryButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16);
	retryButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	retryButton.textureId = ReserveTextureLoadFile("texture/ui/retry.png");
	retryButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/retry_pressed.png");


	retryButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		GoNextScene(GameScene);
	};

	returnTitleButton.pos = retryButton.pos + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16);
	returnTitleButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	returnTitleButton.textureId = ReserveTextureLoadFile("texture/ui/quit.png");
	returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/quit_pressed.png");

	returnTitleButton.releasedCallback = []() {
		GoNextScene(GameStartScene);
	};
	
	gameClearSelect.Add(stageSelectButton);
	gameClearSelect.Add(retryButton);
	gameClearSelect.Add(returnTitleButton);

	backgroundTexture = ReserveTextureLoadFile("texture/background/背景１.png");
	gameClearTexture = ReserveTextureLoadFile("texture/ui/stageclear_1024×256.png");

	gameClearSelect.SetFrame(ReserveTextureLoadFile("texture/ui/select.png"));

	LoadTexture();

	frame = 0;
}

void UninitGameClear()
{
	gameClearSelect.Uninit();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameClearTexture);
}

void DrawGameClear()
{
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });

		auto tPos = D3DXVECTOR2(
			0,
			GAME_CLEAR_LOGO_HEIGHT * (frame / 16 % 4)
		);

	DrawSprite(gameClearTexture, { 200,64 }, 10, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT }, tPos, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT });
	gameClearSelect.Draw();
}

void UpdateGameClear()
{
	gameClearSelect.Update();

	frame++;
}


