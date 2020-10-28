//ゲーム本体
// テストコメント


#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "debugFont.h"
#include "player.h"
#include "GameOperation.h"
#include "game.h"


void InitGame() {
	InitPlayer();
	InitGameOperation();
	LoadTexture();
}

void UpdateGame() {

	UpdateGameOperation();
	UpdatePlayer();

}

void DrawGame() {
	DrawPlayer();
}

void UninitGame() {
	UninitGameOperation();
	UninitPlayer();
}

