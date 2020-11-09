#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"

#ifndef _GAME_CLEAR_H
#define _GAME_CLEAR_H

int gameClearTexture;
void InitGameClear(){
	//gameClearTexture = ReserveTextureLoadFile("texture/gameClear.png");
	LoadTexture();
}
void UninitGameClear(){
	ReleaseTexture(gameClearTexture);
}
void DrawGameClear(){
	DrawSprite(gameClearTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { 1920,1080 });

}
void UpdateGameClear(){
	if (ReleaseInputLogger(MYVK_ENTER)) {
		GoNextScene(GameStartScene);
	}
}
#endif // !_GAME_CLEAR_H

