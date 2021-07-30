#pragma once

// DirectX
#include <d3d9.h>
#include <d3dx9.h>

// DirectShow
#include <stdio.h>
#include <d3d9types.h>
#include <dshow.h>

#include <string>


// lib
#pragma comment( lib, "strmiids.lib" )
//#pragma comment( lib, "strmbasd.lib" )


class Movie
{
public:


	Movie(std::wstring path,D3DXVECTOR2 screenPos=D3DXVECTOR2(0,0),D3DXVECTOR2 size = D3DXVECTOR2(0, 0)) {
		this->path = path;
		this->screenPos = screenPos;

		destR = {};
		destR.right = size.x;
		destR.bottom = size.y;
	}
	~Movie() {
	}

	void Init();
	void Uninit();

	void Play();
	void Stop();

	bool IsPlaying();


	void SetSize(const D3DXVECTOR2& size = D3DXVECTOR2(0, 0));
	void SetPos(const D3DXVECTOR2& screenPos);
private:

	IGraphBuilder* pGB;

	IVMRWindowlessControl* pControl;

	ICaptureGraphBuilder2* pCGB2 = NULL;
	IBaseFilter* pVMR9 = NULL;
	IMediaControl* pMediaCont = NULL;

	HWND hWnd = NULL;

	std::wstring path;
	D3DXVECTOR2 screenPos;


	RECT srcR, destR;
};

