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
	if (PressInputLogger(MYVK_ENTER)) {
		BlockDecision();
	}

}

void DrawInput() {

}

