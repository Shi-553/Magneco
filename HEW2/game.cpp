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

#include "Input.h"

#include "map.h"
#include "flyingObjectSponer.h"



void InitGame() 
{
	InitPlayer();
	InitInput();
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

	UninitInput();

	UpdateNPC();


	UpdateFlyingObject();
	UpdateFlyingSponer();
}

void DrawGame() 
{
	DrawMap();
	DrawPlayer();

	DrawInput();

	DrawNPC();

	DrawFlyingObject();

}

void UninitGame()
{
	UninitGameOperation();
	UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitMap();
}

