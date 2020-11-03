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
#include "map.h"


void InitGame() 
{
	InitPlayer();
	InitGameOperation();
	InitMap();
	LoadTexture();
}

void UpdateGame() 
{

	UpdateMap();
	UpdateGameOperation();
	UpdatePlayer();

}

void DrawGame() 
{
	DrawMap();
	DrawPlayer();
}

void UninitGame()
{
	UninitGameOperation();
	UninitPlayer();
	UninitMap();
}

