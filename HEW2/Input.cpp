// -------------------------------------------------------------------------
//	入力をとる処理[Input.cpp]
// 
//						Author:Kanna Noda
// -------------------------------------------------------------------------

#include "Input.h"
#include "InputLogger.h"
#include "player.h"


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

}

void DrawInput() {

}

