#pragma once
#include <Windows.h>

enum VirtualKey {
	MYVK_UP,
	MYVK_DOWN,
	MYVK_LEFT,
	MYVK_RIGHT,
	MYVK_ENTER,
	MYVK_BEACON,
	MYVK_PURGE,


	MYVK_CONTROL,
	MYVK_PASTE,

	MYVK_LEFT_CLICK,
	MYVK_RIGHT_CLICK,



	MYVK_ESC,

	MYVK_START,
	MYVK_GAME_CLEAR,
	MYVK_GAME_OVER,
	MYVK_STAGE_EDTIOR,
	MYVK_GAME,

	MYVK_SAVE,
	MYVK_LOAD,

	MYVK_SHIFT,

	MYVK_START_STOP,
	MYVK_FIRST_START,
	MYVK_L,

	MYVK_ARROW_UP,
	MYVK_ARROW_DOWN,

	MYVK_ARROW_LEFT,
	MYVK_ARROW_RIGHT,
#if _DEBUG
#endif // _DEBUG

	MYVK_MAX
};

enum VirtualAxis {
	MYVA_MX,
	MYVA_MY,
	MYVA_MW,

	MYVA_GLX,
	MYVA_GLY,
	MYVA_GRX,
	MYVA_GRY,
	MYVA_MAX,
};

void InitInputLogger(HWND hWnd, HINSTANCE hIns);
void UninitInputLogger();
void UpdateInputLogger();

bool PressInputLogger(VirtualKey key);
bool TriggerInputLogger(VirtualKey key);
bool ReleaseInputLogger(VirtualKey key);

int GetInputLoggerAxisInt(VirtualAxis axis);
int GetInputLoggerAxisAmountInt(VirtualAxis axis);
float GetInputLoggerAxis(VirtualAxis axis);
float GetInputLoggerAxisAmount(VirtualAxis axis);


void InputLoggerProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);


void RecordStart(int frameMax);
void RecordEnd();

void RecordLoad();
void TraceStart();
void TraceEnd();
bool IsTrace();


void SetRecordFilename(const char* f, size_t size);


enum class OutputLogType {
	KEYBORAD = 1 << 0,
	MOUSE_BUTTON = 1 << 1,
	MOUSE_AXIS = 1 << 2,
	GAMEPAD_BUTTON = 1 << 3,
	GAMEPAD_AXIS = 1 << 4
};



void DebugPrintInputLogger(OutputLogType type);
