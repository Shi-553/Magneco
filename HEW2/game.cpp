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
#include "gameSrite.h"
#include "game.h"

#include "Input.h"
#include "judge.h"

#include "map.h"
#include "flyingObjectSponer.h"



void InitGame() 
{
	InitPlayer();
	InitInput();
	InitNPC();
	InitMap();
	InitJudge();
	LoadTexture();
	
}

void UpdateGame() 
{

	UpdateMap();
	UpdatePlayer();

	UpdateInput();

	UpdateNPC();
	UpdateJudge();

	UpdateFlyingObject();
	UpdateFlyingSponer();
}

void DrawGame() 
{
	DrawMap();
	DrawPlayer();

	DrawInput();

	DrawNPC();
	DrawJudge();
	DrawFlyingObject();

}

void UninitGame()
{
	UninitInput();
	UninitPlayer();
	UninitJudge();
	UninitNPC();
	UninitMap();
}

