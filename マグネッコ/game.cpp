#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "debugFont.h"
#include "player.h"
#include "npc.h"
#include "effect.h"
#include "gameSrite.h"
#include "game.h"

#include "Input.h"
#include "judge.h"

#include "map.h"
#include "flyingObjectSponer.h"
#include "debugPrintf.h"
#include "importExport.h"
#include "stageInfo.h"
#include "sound.h"
#include "movie.h"


void InitGame() 
{
	InitStageInfo();

	InitPlayer();
	InitInput();
	InitNPC();
	InitEffect();
	InitMap();
	InitJudge();
	InitFlyingObject();
	InitFlyingSponer();
	//Grid_Initialize(GAME_SPRITE_WHIDTH, GetMapWidth(), D3DCOLOR_RGBA(0, 197, 0, 255));

	//StageExport();
	auto info=StageImport();
	if (info != nullptr) {
		StopSound();
		PlaySound(info->sLabel);
	}
	LoadTexture();


}

void UpdateGame() 
{

	UpdateMap();
	UpdatePlayer();

	UpdateInput();

	UpdateNPC();
	UpdateEffect();

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
	DrawFlyingSponer();

	DrawEffect();

	DrawPlayer();

}

void UninitGame()
{
	UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitEffect();
	UninitJudge();
	UninitMap();

	UninitFlyingObject();
	UninitFlyingSponer();

	UninitStageInfo();
}

