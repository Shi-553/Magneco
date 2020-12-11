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

	//十字キー
	GAMEPAD_UP,
	GAMEPAD_DOWN,
	GAMEPAD_LEFT,
	GAMEPAD_RIGHT,

	//その他のボタン
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

	//軸が四方向に傾いてるかどうか
	GAMEPAD_LUP,
	GAMEPAD_LDOWN,
	GAMEPAD_LLEFT,
	GAMEPAD_LRIGHT,
	GAMEPAD_LRUP,
	GAMEPAD_LRDOWN,
	GAMEPAD_LRLEFT,
	GAMEPAD_LRRIGHT,

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

//シングルトンにした
class Gamepad {
public:
	Gamepad(const Gamepad&) = delete;
	Gamepad& operator=(const Gamepad&) = delete;
	Gamepad(Gamepad&&) = delete;
	Gamepad& operator=(Gamepad&&) = delete;

	//みんなこれで生成して使って
	static Gamepad* GetInstance() {
		static Gamepad gamepad;
		return &gamepad;
	}

	bool Init(HWND hWnd, HINSTANCE hIns);
	void Uninit();

	// ゲームパッドの入力情報取得
	bool Update();

	//エラーでfalse
	bool IsButtonDown(GamepadButtons);
	//エラーでfalseにしたいからIsButtonDownと別にした
	bool IsButtonUp(GamepadButtons);

	// -stickMax ~ stickMax
	int GetAxisInt(GamepadAxis);

	// -1.0 ~ 1.0
	float GetAxis(GamepadAxis);

	//GetAxisIntの範囲を決める
	LONG stickMax = 1000;
	//反応しない範囲
	int unresponsiveRange = 200;

private:

	Gamepad() {
	}
	~Gamepad() {
		Uninit();
	}


	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	bool DeviceInit();


	IDirectInput8* inputInterface = NULL;
	IDirectInputDevice8* inputDeveice = NULL;
	HWND hwnd = NULL;

	bool isDataNone = true;
	DIJOYSTATE padData = {};

};