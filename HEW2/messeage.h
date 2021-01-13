#pragma once

#include <d3dx9.h>


void InitMesseage();
void UninitMesseage();

void ClearMesseageOffset();

void SetMessageColor(D3DCOLOR c);

void DrawMesseage(const char* str, va_list argp);
void DrawMesseage(const char* str, ...);

void SetFontMargin(long margin);
void SetFontRectAddY(int addY);
void SetFontScale(D3DXVECTOR2 scale);
void SetFontCountMax(int x, int y);

//デバッグ用
void TestMessage();