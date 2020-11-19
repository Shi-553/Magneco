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
	if (TriggerInputLogger(MYVK_UP)) {
		MoveUpPlayer();
	}
	if (TriggerInputLogger(MYVK_DOWN)) {
		MoveDownPlayer();
	}
	if (TriggerInputLogger(MYVK_LEFT)) {
		MoveLeftPlayer();
	}
	if (TriggerInputLogger(MYVK_RIGHT)) {
		MoveRightPlayer();
	}

	// ブロックの決定（現時点ではpressにしてます）
	if (TriggerInputLogger(MYVK_ENTER)) {
		BlockDecision();
	}
	if (TriggerInputLogger(MYVK_BEACON)) {
		PutBeacon();
	}

#if _DEBUG
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

