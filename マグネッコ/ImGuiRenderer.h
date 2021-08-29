﻿#pragma once

#include "config.h"


class ImGuiRenderer
{
public:
	void Init(HWND hWnd);
	void Uninit();
	LRESULT CALLBACK Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void DrawBegin();
	void DrawEnd();


private:

};

