#include "stageEditor.h"
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
#include "grid.h"
#include "debugPrintf.h"
#include "mapEditor.h"
#include "importExport.h"
#include "InputLogger.h"
#include "sceneManager.h"

void InitStageEditor() {
	InitPlayer();
	//InitInput();
	InitNPC();
	InitMap();
	//InitJudge();
	InitFlyingObject();
	InitFlyingSponer();

	InitMapEditor();

	//Grid_Initialize(GAME_SPRITE_WHIDTH, GetMapWidth(), D3DCOLOR_RGBA(0, 197, 0, 255));

	LoadTexture();

	SetStagePath("stage/edit.stage");

	StageImport();
}

void UninitStageEditor() {
	//UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitMapEditor();

	UninitJudge();
	Grid_Finalize();
	UninitMap();
	UninitFlyingObject();
	UninitFlyingSponer();
}

void DrawStageEditor() {
	DrawMap();
	Grid_Draw();
	//DrawNPC();

	DrawMapEditor();
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
	UpdateMapEditor();
	//UpdateJudge();
	//UpdateFlyingObject();
	//UpdateFlyingSponer();

#if _DEBUG
	if (PressInputLogger(MYVK_CONTROL) && TriggerInputLogger(MYVK_SAVE)) {
		StageExport();

	}
	if (PressInputLogger(MYVK_CONTROL) && TriggerInputLogger(MYVK_LOAD)) {
		StageImport();
	}
#endif // _DEBUG
}

