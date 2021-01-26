#include "Input.h"
#include "InputLogger.h"
#include "player.h"
#include "sceneManager.h"


void InitInput() {

}

void UninitInput() {

}

void UpdateInput() {
	auto gamepadAxis = D3DXVECTOR2(
		GetInputLoggerAxis(MYVA_GLX),
		GetInputLoggerAxis(MYVA_GLY));

	if (gamepadAxis.x != 0 || gamepadAxis.y != 0) {
		MovePlayer(gamepadAxis);
	}
	else {
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
	}

	// ブロックの決定
	if (PressInputLogger(MYVK_ENTER)) {
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


#ifdef _DEBUG
	DebugPrintInputLogger(OutputLogType::GAMEPAD_BUTTON);
#endif // _DEBUG
}

void DrawInput() {

}

