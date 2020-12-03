#pragma once

#include <d3dx9.h>


void InitMessage();
void UninitMessage();

void ClearMessageOffset();

void SetMessageColor(D3DCOLOR c);

void DrawMessage(const char* str, va_list argp);
void DrawMessage(const char* str, ...);
