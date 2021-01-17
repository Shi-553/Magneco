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
#include "debugPrintf.h"
#include "importExport.h"
#include "stageInfo.cpp"


void InitGame() 
{
	InitStageInfo();

	InitPlayer();
	InitInput();
	InitNPC();
	InitMap();
	InitJudge();
	InitFlyingObject();
	InitFlyingSponer();
	//Grid_Initialize(GAME_SPRITE_WHIDTH, GetMapWidth(), D3DCOLOR_RGBA(0, 197, 0, 255));

	LoadTexture();


	StageImport();
	//StageExport();
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
	DrawNPC();
	

	DrawInput();

	
	DrawJudge();
	DrawFlyingObject();

	DrawPlayer();

}

void UninitGame()
{
	UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitJudge();
	UninitMap();

	UninitFlyingObject();
	UninitFlyingSponer();

	UninitStageInfo();
}

