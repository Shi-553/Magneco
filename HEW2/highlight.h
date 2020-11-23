#pragma once
#include "myd3d.h"

void InitHighlight();
void UninitHighlight();
void DrawHighlight(D3DXVECTOR2 pos, float size, int count);
void DrawFrontBlack(D3DXCOLOR color);