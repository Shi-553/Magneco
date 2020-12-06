#pragma once
#include "myd3d.h"

//位置、大きさ、ピボットの位置(0~1)、何角形か、角度（弧度法）
void DrawHighlight(D3DXVECTOR2 pos, D3DXVECTOR2 size, D3DXVECTOR2 pivot, int count, float startDegreeAngle);

void DrawForeground(D3DCOLOR color);