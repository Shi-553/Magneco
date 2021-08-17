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
	void Pause(bool isShow=true);
	void Stop(bool isShow=true);


	bool IsPlaying();


	void SetSize(const D3DXVECTOR2& size = D3DXVECTOR2(0, 0));
	void SetPos(const D3DXVECTOR2& screenPos);
	void SetIsLoop(bool isLoop);

	const RECT& GetSrcRect() {
		return srcR;
	}
	const RECT& GetDestRect() {
		return destR;
	}


	void HandleEvent();
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:

	void SwitchPause(bool sw);

	int eventId;

	bool isLoop;

	bool isPlaying;


	LONGLONG duration;
	FILTER_STATE  filterState;

	IGraphBuilder* pGB;

	IVMRWindowlessControl* pControl;

	ICaptureGraphBuilder2* pCGB2 = NULL;
	IBaseFilter* pVMR9 = NULL;
	IMediaControl* pMediaCont = NULL;
	IMediaSeeking* pMediaSeek = NULL;
	IMediaEventEx* pMediaEvent = NULL;

	HWND hWnd = NULL;

	std::wstring path;
	D3DXVECTOR2 screenPos;
	D3DXVECTOR2 size;


	RECT srcR, destR;


};

