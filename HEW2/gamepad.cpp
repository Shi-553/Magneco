#include "gamepad.h"
#include "debugPrintf.h"



bool Gamepad::Init(HWND hWnd, HINSTANCE hIns) {
	Uninit();

	this->hwnd = hWnd;

	auto hr = DirectInput8Create(hIns,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&inputInterface,
		NULL);

	if (FAILED(hr))
	{
		return false;
	}

	hr = inputInterface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,
		DeviceFindCallBack,
		this,
		DIEDFL_ATTACHEDONLY
	);

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void Gamepad::Uninit() {
	padData = {};
	isDataNone = true;
	hwnd = NULL;

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

BOOL CALLBACK Gamepad::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	auto gamepad = (Gamepad*)pvRef;

	auto hr = gamepad->inputInterface->CreateDevice(
		lpddi->guidInstance,
		&gamepad->inputDeveice,
		NULL
	);

	if (FAILED(hr) || !gamepad->DeviceInit())
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}


bool Gamepad::DeviceInit() {
	auto hr = inputDeveice->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return false;
	}

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

	hr = inputDeveice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hr))
	{
		return false;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -stickMax;
	diprg.lMax = stickMax;

	hr = inputDeveice->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return false;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	hr = inputDeveice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return false;
	}

	// RX軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RX;
	hr = inputDeveice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return false;
	}

	// RY軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RY;
	hr = inputDeveice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return false;
	}

	// 協調モードの設定(バックグラウンドでデバイス取得ができるかどうか、他のアプリと共有するかどうか)
	hr = inputDeveice->SetCooperativeLevel(
		hwnd,
		DISCL_EXCLUSIVE | DISCL_FOREGROUND
	);
	if (FAILED(hr))
	{
		return false;
	}

	//制御開始
	hr = inputDeveice->Acquire();

	if (FAILED(hr))
	{
		return false;
	}

	//ポーリング開始
	hr = inputDeveice->Poll();

	if (FAILED(hr))
	{
		return false;
	}
}



bool Gamepad::Update() {
	if (inputDeveice == NULL) {
		return false;
	}

	HRESULT hr = inputDeveice->GetDeviceState(sizeof(DIJOYSTATE), &padData);

	if (FAILED(hr))
	{
		isDataNone = true;
		inputDeveice->Acquire();
		inputDeveice->Poll();


		return false;
	}

	isDataNone = false;
	return true;
}


//エラーでfalse
bool Gamepad::IsButtonDown(GamepadButtons button) {
	if (isDataNone) {
		return false;
	}

	//十字キー
	if (GAMEPAD_UP <= button && button <= GAMEPAD_RIGHT) {
		if (padData.rgdwPOV[0] == 0xFFFFFFFF)
		{
			return false;
		}

		auto pov = padData.rgdwPOV[0] / 100;

		if (button == GAMEPAD_UP && (pov == 0 || pov == 45 || pov == 315)) {
			return true;
		}

		if (button == GAMEPAD_DOWN && (pov == 135 || pov == 180 || pov == 225)) {
			return true;
		}

		if (button == GAMEPAD_LEFT && (pov == 225 || pov == 270 || pov == 315)) {
			return true;
		}

		if (button == GAMEPAD_RIGHT && (pov == 45 || pov == 90 || pov == 135)) {
			return true;
		}

		return false;
	}

	//その他のボタン
	else if (GAMEPAD_XBUTTON1 <= button && button <= GAMEPAD_XBUTTON31) {
		return padData.rgbButtons[button - GAMEPAD_XBUTTON1] & 0x80;
	}

	//軸が四方向に傾いてるかどうか
	else if (GAMEPAD_LUP <= button && button <= GAMEPAD_LRRIGHT) {
		GamepadAxis axis;
		if (button == GAMEPAD_LUP || button == GAMEPAD_LDOWN) {
			axis = GAMEPAD_LY;
		}
		else if (button == GAMEPAD_LLEFT || button == GAMEPAD_LRIGHT) {
			axis = GAMEPAD_LX;
		}
		else if (button == GAMEPAD_LRUP || button == GAMEPAD_LRDOWN) {
			axis = GAMEPAD_LRY;
		}
		else if (button == GAMEPAD_LRLEFT || button == GAMEPAD_LRRIGHT) {
			axis = GAMEPAD_LRX;
		}

		if (button == GAMEPAD_LUP || button == GAMEPAD_LLEFT || button == GAMEPAD_LRUP || button == GAMEPAD_LRLEFT) {
			return 0 > GetAxisInt(axis);
		}
		else {
			return 0 < GetAxisInt(axis);
		}
	}

	return false;
}

//エラーでfalseにしたいからIsButtonDownと別にした
bool Gamepad::IsButtonUp(GamepadButtons button) {
	if (isDataNone) {
		return false;
	}

	//十字キー
	if (GAMEPAD_UP <= button && button <= GAMEPAD_RIGHT) {
		if (padData.rgdwPOV[0] == 0xFFFFFFFF)
		{
			return false;
		}

		auto pov = padData.rgdwPOV[0] / 100;

		if (button == GAMEPAD_UP && (pov == 0 || pov == 45 || pov == 315)) {
			return false;
		}

		if (button == GAMEPAD_DOWN && (pov == 135 || pov == 180 || pov == 225)) {
			return false;
		}

		if (button == GAMEPAD_LEFT && (pov == 225 || pov == 270 || pov == 315)) {
			return false;
		}

		if (button == GAMEPAD_RIGHT && (pov == 45 || pov == 90 || pov == 135)) {
			return false;
		}

		return true;
	}

	//その他のボタン
	else if (GAMEPAD_XBUTTON1 <= button && button <= GAMEPAD_XBUTTON31) {
		return !(padData.rgbButtons[button - GAMEPAD_XBUTTON1] & 0x80);
	}

	//軸が四方向に傾いてるかどうか
	else if (GAMEPAD_LUP <= button && button <= GAMEPAD_LRRIGHT) {
		GamepadAxis axis;
		if (button == GAMEPAD_LUP || button == GAMEPAD_LDOWN) {
			axis = GAMEPAD_LY;
		}
		else if (button == GAMEPAD_LLEFT || button == GAMEPAD_LRIGHT) {
			axis = GAMEPAD_LX;
		}
		else if (button == GAMEPAD_LRUP || button == GAMEPAD_LRDOWN) {
			axis = GAMEPAD_LRY;
		}
		else if (button == GAMEPAD_LRLEFT || button == GAMEPAD_LRRIGHT) {
			axis = GAMEPAD_LRX;
		}

		if (button == GAMEPAD_LUP || button == GAMEPAD_LLEFT || button == GAMEPAD_LRUP || button == GAMEPAD_LRLEFT) {
			return 0 <= GetAxisInt(axis);
		}
		else {
			return 0 >= GetAxisInt(axis);
		}
	}

	return false;
}

int Gamepad::GetAxisInt(GamepadAxis axis) {
	if (isDataNone) {
		return 0;
	}
	float temp = 0;
	switch (axis)
	{
	case GAMEPAD_LX:
		temp = padData.lX;
		break;

	case GAMEPAD_LY:
		temp = padData.lY;
		break;

	case GAMEPAD_LZ:
		temp = padData.lZ;
		break;

	case GAMEPAD_LRX:
		temp = padData.lRx;
		break;

	case GAMEPAD_LRY:
		temp = padData.lRy;
		break;

	case GAMEPAD_LRZ:
		temp = padData.lRz;
		break;

	case GAMEPAD_RGL_SLIDER1:
		temp = padData.rglSlider[0];
		break;

	case GAMEPAD_RGL_SLIDER2:
		temp = padData.rglSlider[1];
		break;

	default:
		return 0;
	}

	temp = temp * 10.0f / 7.0f;

	if (-unresponsiveRange < temp && temp < unresponsiveRange) {
		return 0;
	}
	if (-stickMax > temp) {
		return -stickMax;
	}
	if (stickMax < temp) {
		return stickMax;
	}
	return temp;
}

float Gamepad::GetAxis(GamepadAxis axis) {
	return GetAxisInt(axis) / (float)stickMax;
}