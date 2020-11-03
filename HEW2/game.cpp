//ゲーム本体
// テストコメント
// テスト２

//リモート・マスターからブランチ分けて追加
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "debugFont.h"
#include "player.h"
#include "GameOperation.h"
#include "game.h"
#include "Input.h"


void InitGame() {
	InitPlayer();
	InitInput();
	InitGameOperation();
	LoadTexture();
}

void UpdateGame() {

	UpdateGameOperation();
	UpdatePlayer();
	UninitInput();

}

void DrawGame() {
	DrawPlayer();
	DrawInput();
}

void UninitGame() {
	UninitGameOperation();
	UninitInput();
	UninitPlayer();
}

