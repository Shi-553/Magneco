// -------------------------------------------------------------------------
//	入力をとる処理[Input.cpp]
// 
//						Author:Kanna Noda
// -------------------------------------------------------------------------

#include "Input.h"
#include "InputLogger.h"
#include "player.h"
#include "sceneManager.h"


void InitInput() {

}

void UninitInput(){

}

void UpdateInput() {

	// プレイヤーの移動
	if (PressInputLogger(MYVK_UP)) {
		MoveUpPlayer();
	}
	if (PressInputLogger(MYVK_DOWN)) {
		MoveDownPlayer();
	}
	if (PressInputLogger(MYVK_LEFT)) {
		MoveLeftPlayer();
	}
	if (PressInputLogger(MYVK_RIGHT)) {
		MoveRightPlayer();
	}

	// ブロックの決定（現時点ではpressにしてます）
	if (TriggerInputLogger(MYVK_ENTER)) {
		BlockDecision();
	}
	if (TriggerInputLogger(MYVK_BEACON)) {
		PutBeacon();
	}
	if (TriggerInputLogger(MYVK_PURGE)) {
		PurgePlayerFlyingObject();
	}

#if _DEBUG
	DebugPrintInputLogger();
	if (TriggerInputLogger(MYVK_GAME_CLEAR)) {
		GoNextScene(GameClearScene);
	}

	if (TriggerInputLogger(MYVK_GAME_OVER)) {
		GoNextScene(GameOverScene);
	}
#endif
}

void DrawInput() {

}

