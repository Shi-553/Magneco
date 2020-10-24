
#ifndef _MY_D3D_H
#define _MY_D3D_H

#include <d3d9.h>
#include <d3dx9.h>

//–ß‚è’l Ž¸”s:false
bool InitMyD3D(HWND hWnd);

void UninitMyD3D();

LPDIRECT3DDEVICE9 GetD3DDevice();
#endif // !_MYD3D_H
