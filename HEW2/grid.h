#ifndef GRID_H_
#define GRID_H_

#include <d3d9.h>

// グリッド描画モジュールの初期化
//
// size  ... グリッドサイズ
// count ... 一辺のグリッド数
// color ... グリッドカラー

void Grid_Initialize(float size, int count, D3DCOLOR color);

void Grid_Finalize(void);

void Grid_Draw(void);

#endif // !GRID_H_
