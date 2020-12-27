
#include "config.h"
#include "sprite.h"
#include "texture.h"
#include "sceneManager.h"

#include "gameStart.h"
#include "game.h"
#include "gameOver.h"
#include "gameClear.h"
#include "tutorial.h"
#include "stageEditor.h"
#include "InputLogger.h"
#include "importExport.h"


typedef void (*SceneFunc)(void);


static const SceneFunc Inits[]{
		InitGameStart,
		InitGame,
		InitGameClear,
		InitGameOver,
		InitTutorial,
		InitStageEditor
};

static const SceneFunc Uninits[]{
		UninitGameStart,
		UninitGame,
		UninitGameClear,
		UninitGameOver,
		UninitTutorial,
		UninitStageEditor
};

static const SceneFunc Updates[]{
		UpdateGameStart,
		UpdateGame,
		UpdateGameClear,
		UpdateGameOver,
		UpdateTutorial,
		UpdateStageEditor
};

static const SceneFunc Draws[]{
		DrawGameStart,
		DrawGame,
		DrawGameClear,
		DrawGameOver,
		DrawTutorial,
		DrawStageEditor
};

Scene currentScene;
Scene nextScene;
float fade;
FadeMode fadeMode;

int fadeTexture;

void InitSceneManager(Scene startScene) {
	fadeTexture = ReserveTextureLoadFile("texture/fade.png");

	currentScene = nextScene = NullScene;
	fadeMode = FADE_NONE;
	GoNextScene(startScene, FADE_IN);

	SetStagePath("stage/edit.stage");
}

void UninitSceneManager() {
	if (NullScene < currentScene && currentScene < MaxScene) {
		Uninits[currentScene]();
	}
}

void DrawSceneManager() {
	if (NullScene < currentScene && currentScene < MaxScene) {
		Draws[currentScene]();
	}
	if (fade > 0) {
		int a = (int)(fade * 255);
		if (a > 255)
			a = 255;
		SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, a));
		DrawSprite(fadeTexture, { 0,0 }, -10, { SCREEN_WIDTH,SCREEN_HEIGHT });

		SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

void UpdateSceneManager() {

	//フェードアウト
	if (fadeMode == FADE_OUT) {

		fade += 0.02f;

		if (1 > fade) {
			return;
		}
		//完全に真っ黒になったとき
		if (NullScene < currentScene && currentScene < MaxScene) {
			Uninits[currentScene]();
		}
		if (NullScene < nextScene && nextScene < MaxScene) {
			Inits[nextScene]();
		}

		currentScene = nextScene;
		fadeMode = FADE_IN;

		return;
	}

	//フェードイン
	if (fadeMode == FADE_IN) {
		fade -= 0.02f;

		if (fade <= 0.2f) {
			fade -= 0.04f;
		}
		if (fade < 0) {
			fadeMode = FADE_NONE;
		}
		return;
	}

	//通常
	if (NullScene < currentScene && currentScene < MaxScene) {
		Updates[currentScene]();
	}

#ifdef _DEBUG


	if (PressInputLogger(MYVK_START)) {
		GoNextScene(GameStartScene);
	}
	if (PressInputLogger(MYVK_GAME)) {
		GoNextScene(GameScene);
	}

	if (TriggerInputLogger(MYVK_STAGE_EDTIOR)) {
		GoNextScene(StageEditorScene);
	}

	if (TriggerInputLogger(MYVK_GAME_CLEAR)) {
		GoNextScene(GameClearScene);
	}

	if (TriggerInputLogger(MYVK_GAME_OVER)) {
		GoNextScene(GameOverScene);
	}
#endif // _DEBUG

}

void GoNextScene(Scene scene, FadeMode mode) {
	if (scene == currentScene)
		return;

	if (mode == FADE_IN) {
		fade = 1;
	}
	if (mode == FADE_OUT) {
		fade = 0;
	}

	fadeMode = FADE_OUT;
	nextScene = scene;
}
