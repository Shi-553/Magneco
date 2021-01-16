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
		PutCansel();
	}
	if (PressInputLogger(MYVK_DOWN)) {
		MoveDownPlayer();
		PutCansel();
	}
	if (PressInputLogger(MYVK_LEFT)) {
		MoveLeftPlayer();
		PutCansel();
	}
	if (PressInputLogger(MYVK_RIGHT)) {
		MoveRightPlayer();
		PutCansel();
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
		PutCansel();
	}

	// ブロックの決定
	if (TriggerInputLogger(MYVK_ENTER)) {
		MakePut();
	}
	if (ReleaseInputLogger(MYVK_ENTER)) {
		PutCansel();
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

#ifdef _DEBUG
	//DebugPrintInputLogger(OutputLogType::MOUSE_AXIS);
#endif // _DEBUG
}

void DrawInput() {

}

