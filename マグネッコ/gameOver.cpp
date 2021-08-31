
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameOver.h"
#include "sceneManager.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "sound.h"
#include "stageSelect.h"

#define GAME_OVER_BUTTON_WIDTH 330
#define GAME_OVER_BUTTON_HEIGHT 90

#define GAME_OVER_LOGO_WIDTH 896
#define GAME_OVER_LOGO_HEIGHT 224

static int backgroundTexture;
static int gameOverTextTexture;

static SelectButton gameOverSelect;

void InitGameOver() {

	StopSound();
	PlaySound(SOUND_LABEL_BGM004);

	gameOverSelect.Init();

Button retryButton, stageSelectButton, returnTitleButton;

	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_OVER_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_OVER_BUTTON_HEIGHT / 2) + 30);

	retryButton.pos = buttonCenter;
	retryButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	retryButton.textureId = ReserveTextureLoadFile("texture/ui/retry.png");
	retryButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/retry_pressed.png");

	retryButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		if (GetStageFoldername() == "stage/main") {
			GoNextScene(GameScene);
		}
		else {
			GoNextScene(TutorialScene);
		}
	};

	stageSelectButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_OVER_BUTTON_HEIGHT + 16);
	stageSelectButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	stageSelectButton.textureId = ReserveTextureLoadFile("texture/ui/stageselect.png");
	stageSelectButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/stageselect_pressed.png");

	stageSelectButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		GoNextScene(StageSelect);
	};

	returnTitleButton.pos = stageSelectButton.pos + D3DXVECTOR2(0, GAME_OVER_BUTTON_HEIGHT + 16);
	returnTitleButton.size = D3DXVECTOR2(GAME_OVER_BUTTON_WIDTH, GAME_OVER_BUTTON_HEIGHT);
	returnTitleButton.textureId = ReserveTextureLoadFile("texture/ui/quit.png");
	returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/quit_pressed.png");

	returnTitleButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		GoNextScene(GameStartScene);
	};

	gameOverSelect.Add(retryButton);
	gameOverSelect.Add(stageSelectButton);
	gameOverSelect.Add(returnTitleButton);

	backgroundTexture = ReserveTextureLoadFile("texture/background/背景2.jpg");

	gameOverTextTexture = ReserveTextureLoadFile("texture/ui/gameover.png");

	gameOverSelect.SetFrame(ReserveTextureLoadFile("texture/ui/select.png"));

	LoadTexture();
}

void UninitGameOver() {
	gameOverSelect.Uninit();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameOverTextTexture);

}
void DrawGameOver() {
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(gameOverTextTexture, { 200,64 }, 10, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT }, { 0,0 }, { GAME_OVER_LOGO_WIDTH,GAME_OVER_LOGO_HEIGHT });
	gameOverSelect.DrawMain();
	gameOverSelect.DrawLR();
}

void UpdateGameOver()
{
	gameOverSelect.Update();
}