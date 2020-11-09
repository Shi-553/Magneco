#include "gameOver.h"
#include "texture.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "config.h"

int gameOverTexture;
void InitGameOver() {
	gameOverTexture = ReserveTextureLoadFile("texture/gameOver.png");
	LoadTexture();
}
void UninitGameOver() {
	ReleaseTexture(gameOverTexture);
}
void DrawGameOver() {
	DrawSprite(gameOverTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });

}
void UpdateGameOver() {
	if (ReleaseInputLogger(MYVK_ENTER)) {
		GoNextScene(GameStartScene);
	}
}