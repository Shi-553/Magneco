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

	//if (GetInputLoggerAxis(MYVA_GLY)<0) {
	//	MoveUpPlayer();
	//}
	//if (GetInputLoggerAxis(MYVA_GLY) > 0) {
	//	MoveDownPlayer();
	//}
	//if (GetInputLoggerAxis(MYVA_GLX) < 0) {
	//	MoveLeftPlayer();
	//}
	//if (GetInputLoggerAxis(MYVA_GLX) > 0) {
	//	MoveRightPlayer();
	//}
	MovePlayer(D3DXVECTOR2(
		GetInputLoggerAxis(MYVA_GLX),
		GetInputLoggerAxis(MYVA_GLY) ));

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
	if (ReleaseInputLogger(MYVK_PAUSE)) {
		PostQuitMessage(0);
	}

#if _DEBUG
	DebugPrintInputLogger(OutputLogType::GAMEPAD_AXIS);

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

