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
#include "grid.h"
#include "messeage.h"
#include "debugPrintf.h"


void InitGame() 
{
	InitPlayer();
	InitInput();
	InitNPC();
	InitMap();
	InitJudge();
	InitFlyingObject();
	InitFlyingSponer();
	Grid_Initialize(GAME_SPRITE_WHIDTH, MAPCHIP_WIDTH, D3DCOLOR_RGBA(0, 197, 0, 255));
	InitMesseage();

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
	UpdateMesseage();
}

void DrawGame() 
{
	DrawMap();
	Grid_Draw();
	DrawPlayer();

	DrawInput();

	DrawNPC();
	DrawJudge();
	DrawFlyingObject();
	DrawMesseage("ああ", D3DCOLOR_RGBA(255, 255, 255, 255));
	DrawMesseage("ああ\nああ", D3DCOLOR_RGBA(255, 0, 0, 255));
	DrawMesseage("ああ", D3DCOLOR_RGBA(255, 255, 255, 255));

	//DebugPrintf("あああああ");
}

void UninitGame()
{
	UninitMesseage();
	UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitJudge();
	Grid_Finalize();
	UninitMap();

	UninitFlyingObject();
	UninitFlyingSponer();
}

