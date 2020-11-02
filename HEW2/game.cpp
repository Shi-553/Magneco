//ゲーム本体
// テストコメント
// テスト２

//リモート・マスターからブランチ分けて追加
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "debugFont.h"
#include "player.h"
#include "game.h"


void InitGame() {
	InitPlayer();
	LoadTexture();
}

void UpdateGame() {

	UpdatePlayer();

}

void DrawGame() {
	DrawPlayer();
}

void UninitGame() {
	UninitPlayer();
}

