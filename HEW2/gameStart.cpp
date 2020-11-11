
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameStart.h"
#include "sceneManager.h"
#include "InputLogger.h"

int titleTexture;

void InitGameStart() {
	titleTexture = ReserveTextureLoadFile("texture/title.png");
	LoadTexture();
}

void UninitGameStart() {
	ReleaseTexture(titleTexture);

}
void DrawGameStart() {
	DrawSprite(titleTexture, { 0,0 }, 10, {SCREEN_WIDTH,SCREEN_HEIGHT}, {0,0}, { SCREEN_WIDTH,SCREEN_HEIGHT });
}

void UpdateGameStart() {
	if (ReleaseInputLogger(MYVK_ENTER)) {
		GoNextScene(GameScene);
	}
}
