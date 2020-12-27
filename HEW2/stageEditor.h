#pragma once
#include "myd3d.h"

void InitStageEditor();
void UninitStageEditor();
void DrawStageEditor();
void UpdateStageEditor();
bool CheckSquare(const D3DXVECTOR2& target, const D3DXVECTOR2& leftUpPos, const  D3DXVECTOR2& size);
