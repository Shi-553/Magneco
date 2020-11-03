//ゲーム本体
// テストコメント
// テスト２

//リモート・マスターからブランチ分けて追加
#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "debugFont.h"
#include "player.h"
#include "npc.h"
#include "GameOperation.h"
#include "game.h"
#include "map.h"
#include "flyingObjectSponer.h"


void InitGame() 
{
	InitPlayer();
	InitGameOperation();
	InitNPC();
	InitMap();
	LoadTexture();
	
}

void UpdateGame() 
{

	UpdateMap();
	UpdateGameOperation();
	UpdatePlayer();
	UpdateNPC();

	UpdateFlyingObject();
	UpdateFlyingSponer();
}

void DrawGame() 
{
	DrawMap();
	DrawPlayer();
	DrawNPC();

	DrawFlyingObject();
}

void UninitGame()
{
	UninitGameOperation();
	UninitPlayer();
	UninitNPC();
	UninitMap();
}

