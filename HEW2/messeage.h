#pragma once

#include <d3dx9.h>


void InitMesseage();
void UninitMesseage();

void ClearMesseageOffset();

void SetMessageColor(D3DCOLOR c);

void DrawMesseage(const char* str, va_list argp);
void DrawMesseage(const char* str, ...);

void SetFontMargin(long margin);
void SetFontScale(D3DXVECTOR2 scale);
void SetFontPos(D3DXVECTOR2 pos);

//デバッグ用
void TestMessage();