﻿#include "stageEditor.h"
#include "map.h"
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
#include "mapEditor.h"
#include "importExport.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "flyingObjectEditor.h"
#include "stageInfoEditor.h"
#include <string>



void InitStageEditor() {
	InitPlayer();
	//InitInput();
	InitNPC();
	InitMap();
	//InitJudge();
	InitFlyingObject();
	InitFlyingSponer();

	InitMapEditor();
	InitFlyingObjectEditor();
	InitStageInfoEditor();
	//Grid_Initialize(GAME_SPRITE_WHIDTH, GetMapWidth(), D3DCOLOR_RGBA(0, 197, 0, 255));

	LoadTexture();

	//SetStagePath("stage/edit.stage");

	StageImport();

}

void UninitStageEditor() {
	//UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitMapEditor();
	UninitFlyingObjectEditor();

	UninitJudge();
	UninitMap();
	UninitFlyingObject();
	UninitFlyingSponer();
	UninitStageInfoEditor();
}

void DrawStageEditor() {
	DrawMap();
	//DrawNPC();

	DrawMapEditor();
	DrawFlyingObjectEditor();
	DrawStageInfoEditor();
	//DrawInput();


	//DrawJudge();
	//DrawFlyingObject();

	//DrawPlayer();

}

void UpdateStageEditor() {
	UpdateMap();
	//UpdatePlayer();

	//UpdateInput();

	//UpdateNPC();
	UpdateFlyingObjectEditor();
	UpdateMapEditor();
	UpdateStageInfoEditor();

	if (CheckMouseFlyingObjectEditor()) {
		return;
	}

	if (CheckMouseMapEditor()) {
		return;
	}
	if (CheckMouseStageInfoEditor()) {
		return;
	}
	//UpdateJudge();
	//UpdateFlyingObject();
	//UpdateFlyingSponer();

}



bool CheckSquare(const D3DXVECTOR2& target, const D3DXVECTOR2& leftUpPos, const  D3DXVECTOR2& size) {
	return (leftUpPos.x < target.x) && (target.x < leftUpPos.x + size.x) &&
		(leftUpPos.y < target.y) && (target.y < leftUpPos.y + size.y);
}