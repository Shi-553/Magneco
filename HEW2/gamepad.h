//https://yttm-work.jp/directx/directx_0024.html
//見て書いた

#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include "mouse.h"


enum GamepadButtons
{
	GAMEPAD_BUTTONS_NONE = MOUSE_AXIS_MAX,
	GAMEPAD_UP,
	GAMEPAD_DOWN,
	GAMEPAD_LEFT,
	GAMEPAD_RIGHT,
	GAMEPAD_XBUTTON1,
	GAMEPAD_XBUTTON2,
	GAMEPAD_XBUTTON3,
	GAMEPAD_XBUTTON4,
	GAMEPAD_XBUTTON5,
	GAMEPAD_XBUTTON6,
	GAMEPAD_XBUTTON7,
	GAMEPAD_XBUTTON8,
	GAMEPAD_XBUTTON9,
	GAMEPAD_XBUTTON10,
	GAMEPAD_XBUTTON11,
	GAMEPAD_XBUTTON12,
	GAMEPAD_XBUTTON13,
	GAMEPAD_XBUTTON14,
	GAMEPAD_XBUTTON15,
	GAMEPAD_XBUTTON16,
	GAMEPAD_XBUTTON17,
	GAMEPAD_XBUTTON18,
	GAMEPAD_XBUTTON19,
	GAMEPAD_XBUTTON21,
	GAMEPAD_XBUTTON22,
	GAMEPAD_XBUTTON23,
	GAMEPAD_XBUTTON24,
	GAMEPAD_XBUTTON25,
	GAMEPAD_XBUTTON26,
	GAMEPAD_XBUTTON27,
	GAMEPAD_XBUTTON28,
	GAMEPAD_XBUTTON29,
	GAMEPAD_XBUTTON30,
	GAMEPAD_XBUTTON31,
	GAMEPAD_BUTTONS_MAX
};
enum GamepadAxis
{
	GAMEPAD_AXIS_NONE = GAMEPAD_BUTTONS_MAX,
	GAMEPAD_LX,
	GAMEPAD_LY,
	GAMEPAD_LZ,
	GAMEPAD_LRX,
	GAMEPAD_LRY,
	GAMEPAD_LRZ,
	GAMEPAD_RGL_SLIDER1,
	GAMEPAD_RGL_SLIDER2,

	GAMEPAD_AXIS_MAX
};


class Gamepad {
public:
	Gamepad(const Gamepad&) = delete;
	Gamepad& operator=(const Gamepad&) = delete;
	Gamepad(Gamepad&&) = delete;
	Gamepad& operator=(Gamepad&&) = delete;

	static Gamepad* GetInstance() {
		static Gamepad gamepad;
		return &gamepad;
	}

	bool Init(HWND hWnd, HINSTANCE hIns);

	// ゲームパッドの入力情報取得
	bool Update();

	bool IsButtonDown(GamepadButtons);
	bool IsButtonUp(GamepadButtons);

	// -stickMax ~ stickMax
	int GetAxisInt(GamepadAxis);

	// -1.0 ~ 1.0
	float GetAxis(GamepadAxis);

	int stickMax = 1000;
	int unresponsiveRange = 200;

private:

	Gamepad() {
		ZeroMemory(&padData,sizeof(padData));
	}
	~Gamepad() {
		if (inputDeveice != NULL) {
			inputDeveice->Unacquire();
			inputDeveice->Release();
			inputDeveice = NULL;
		}
		if (inputInterface != NULL) {
			inputInterface->Release();
			inputInterface = NULL;
		}
	}


	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	bool DeviceInit();


	IDirectInput8* inputInterface = NULL;
	IDirectInputDevice8* inputDeveice = NULL;
	HWND hwnd = NULL;

	bool isDataNone = true;
	DIJOYSTATE padData;

};