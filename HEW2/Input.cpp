#include "Input.h"
#include "InputLogger.h"
#include "player.h"
#include "sceneManager.h"


void InitInput() {

}

void UninitInput() {

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
	auto gamepadAxis = D3DXVECTOR2(
		GetInputLoggerAxis(MYVA_GLX),
		GetInputLoggerAxis(MYVA_GLY));

	if (gamepadAxis.x != 0 || gamepadAxis.y != 0) {
		MovePlayer(gamepadAxis);
	}

	Player* player = GetPlayer();
	// ブロックの決定（現時点ではpressにしてます）
	if (PressInputLogger(MYVK_ENTER)) {
		player->blockPutFrame++;
		if (player->blockPutFrame >= BLOCK_PUT_FRAME) {
			BlockDecision();
		}
	}
	else {
		player->blockPutFrame = 0;
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

