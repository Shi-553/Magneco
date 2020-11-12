#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"


int gameClearTexture;

void InitGameClear()
{
	gameClearTexture = ReserveTextureLoadFile("texture/”wŒi‚P.png");
	LoadTexture();
}

void UninitGameClear()
{
	ReleaseTexture(gameClearTexture);
}

void DrawGameClear()
{
	DrawSprite(gameClearTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });

}

void UpdateGameClear()
{
	if (ReleaseInputLogger(MYVK_ENTER)) {
		GoNextScene(GameStartScene);
	}
	if (ReleaseInputLogger(MYVK_BEACON)) {
		GoNextScene(GameScene);
	}
}


