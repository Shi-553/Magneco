
#include "config.h"
#include "sprite.h"
#include "texture.h"
#include "sceneManager.h"

#include "gameStart.h"
#include "game.h"
#include "gameOver.h"
#include "gameClear.h"
#include "tutorial.h"


typedef void (*SceneFunc)(void);


static const SceneFunc Inits[]{
		InitGameStart,
		InitGame,
		InitGameClear,
		InitGameOver,
		InitTutorial
};

static const SceneFunc Uninits[]{
		UninitGameStart,
		UninitGame,
		UninitGameClear,
		UninitGameOver,
		UninitTutorial
};

static const SceneFunc Updates[]{
		UpdateGameStart,
		UpdateGame,
		UpdateGameClear,
		UpdateGameOver,
		UpdateTutorial
};

static const SceneFunc Draws[]{
		DrawGameStart,
		DrawGame,
		DrawGameClear,
		DrawGameOver,
		DrawTutorial
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
}

void UninitSceneManager() {
	Uninits[currentScene]();
}

void DrawSceneManager() {
	Draws[currentScene]();

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
		Uninits[currentScene]();
		Inits[nextScene]();

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
	Updates[currentScene]();
}

void GoNextScene(Scene scene, FadeMode mode) {
	if (scene == currentScene)
		return;

	if (mode == FADE_NONE || mode == FADE_IN) {
		if (NullScene < currentScene && currentScene < MaxScene) {
			Uninits[currentScene]();
		}
		Inits[scene]();
		currentScene = scene;
	}

	if (mode == FADE_IN) {
		fade = 1;
	}
	if (mode == FADE_OUT) {
		fade = 0;
	}

	fadeMode = mode;
	nextScene = scene;
}
