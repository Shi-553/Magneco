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

	MYVK_PAUSE,

#if _DEBUG
	MYVK_GAME_CLEAR,
	MYVK_GAME_OVER,
#endif // _DEBUG

	MYVK_MAX
};

enum VirtualAxis {
	MYVA_MX,
	MYVA_MY,
	MYVA_GLX,
	MYVA_GLY,
	MYVA_GRX,
	MYVA_GRY,
	MYVA_MAX
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
inline OutputLogType operator~ (OutputLogType a) { return (OutputLogType)~(int)a; }
inline OutputLogType operator| (OutputLogType a, OutputLogType b) { return (OutputLogType)((int)a | (int)b); }
inline OutputLogType operator& (OutputLogType a, OutputLogType b) { return (OutputLogType)((int)a & (int)b); }
inline OutputLogType operator^ (OutputLogType a, OutputLogType b) { return (OutputLogType)((int)a ^ (int)b); }
inline OutputLogType& operator|= (OutputLogType& a, OutputLogType b) { return (OutputLogType&)((int&)a |= (int)b); }
inline OutputLogType& operator&= (OutputLogType& a, OutputLogType b) { return (OutputLogType&)((int&)a &= (int)b); }
inline OutputLogType& operator^= (OutputLogType& a, OutputLogType b) { return (OutputLogType&)((int&)a ^= (int)b); }
inline bool operator! (OutputLogType a) { return !(bool)a; }

//フラグがどちらかたってたらTRUE
inline bool operator+ (OutputLogType a, OutputLogType b) { return ((int)a | (int)b); }
//フラグが両方たってたらTRUE
inline bool operator* (OutputLogType a, OutputLogType b) { return ((int)a & (int)b); }


void DebugPrintInputLogger(OutputLogType type);
