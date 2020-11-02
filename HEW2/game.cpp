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
#include "flyingObject.h"
#include "flyingObjectSponer.h"


void InitGame() {
	InitPlayer();
	InitGameOperation();

	InitFlyingObject();
	InitFlyingSponer();

	LoadTexture();
}

void UpdateGame() {

	UpdateGameOperation();
	UpdatePlayer();

	UpdateFlyingObject();
	UpdateFlyingSponer();
}

void DrawGame() {
	DrawPlayer();

	DrawFlyingObject();
}

void UninitGame() {
	UninitGameOperation();
	UninitPlayer();

	UninitFlyingObject();
	UninitFlyingSponer();
}

