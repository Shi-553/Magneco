#include "movie.h"
#include <Vmr9.h>
#include "config.h"


#define WM_DIRECTSHOWMESSAGE (WM_APP + 1)
#define MOVIE_WINDOW_CLASS_NAME "MovieWindow"


void Movie::Init() {
	auto instanceHandle = GetModuleHandle(nullptr);



	WNDCLASS wc = {};
	wc.lpfnWndProc = Movie::StaticWndProc; // ウィンドウプロシージャ
	wc.lpszClassName = MOVIE_WINDOW_CLASS_NAME;                     //オリジナルパーツのクラス名
	wc.hInstance = instanceHandle;                          //インスタンスハンドルを登録
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          //マウルカーソルの指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); //背景色の設定   COLOR_BACKGROUND 系は +1 しないといけない


	RegisterClass(&wc);



	DWORD windowsStyle = WS_CHILD ^ WS_EX_NOPARENTNOTIFY;

	LONG windowWidth = destR.right - destR.left;
	LONG windowHeight = destR.bottom - destR.top;



	//ウインドウ作る
	//ハンドル = ユニークなID
	hWnd = CreateWindow(
	  MOVIE_WINDOW_CLASS_NAME,              //ウインドウ クラス
	  "",          //ウインドウ テキスト(上にでてくる)
	  windowsStyle,     //ウインドウ スタイル

	  //サイズと位置
	  screenPos.x,   //画面の左からの自身の左の座標
	  screenPos.y,   //画面の上からの自身の上の座標

	  windowWidth,   //ウインドウの幅
	  windowHeight,   //ウインドウの高さ (フレームを含む)

	  GetActiveWindow(),            //親のウインドウハンドル   
	  NULL,            //メニューハンドル         ↑「ファイル 編集 表示 プロジェクト...」
	  instanceHandle,      //インスタンスハンドル
	  this
	);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);


	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGB);
	if (FAILED(hr))
	{
		printf("ERROR - Could not create the Filter Graph Manager.");
		return;
	}

	CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pVMR9);

	pGB->AddFilter(pVMR9, L"VMR9");

	IVMRFilterConfig* pVMRCfg = NULL;
	pVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&pVMRCfg);
	pVMRCfg->SetRenderingMode(VMRMode_Windowless);

	pVMRCfg->Release();

	pVMR9->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pControl);
	pControl->SetVideoClippingWindow(hWnd);


	IBaseFilter* pSource = NULL;
	pGB->AddSourceFilter(path.c_str(), path.c_str(), &pSource);

	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pCGB2);
	pCGB2->SetFiltergraph(pGB);

	pCGB2->RenderStream(0, 0, pSource, 0, pVMR9);
	pCGB2->RenderStream(0, &MEDIATYPE_Audio, pSource, 0, 0);

	// 描画領域の設定（接続後でないとエラーになる）
	LONG W, H;
	pControl->GetNativeVideoSize(&W, &H, NULL, NULL);


	SetRect(&srcR, 0, 0, W, H);
	GetClientRect(hWnd, &destR);

	pControl->SetVideoPosition(&srcR, &destR);

	if (destR.right == 0 && destR.bottom == 0) {
		SetSize();
	}


	pGB->QueryInterface(IID_IMediaControl, (void**)&pMediaCont);
	pGB->QueryInterface(IID_IMediaSeeking, (void**)&pMediaSeek);
	pMediaSeek->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);

	pMediaSeek->GetDuration(&duration);

	pGB->QueryInterface(IID_IMediaEventEx, (void**)&pMediaEvent);

	pMediaEvent->SetNotifyWindow((OAHWND)hWnd, WM_DIRECTSHOWMESSAGE, (LPARAM)NULL);
}

void Movie::Uninit() {
	if (pMediaCont == nullptr) {
		return;
	}
	pMediaCont->Stop();

	DestroyWindow(hWnd);
	pMediaCont->Release();
	pMediaCont = nullptr;

	pControl->Release();

	pVMR9->Release();
	pCGB2->Release();

	pGB->Release();
}
void Movie::Play() {

	pMediaCont->Run();

}
void Movie::Stop() {
	if (pMediaCont == nullptr) {
		return;
	}

	pMediaCont->Stop();
}

bool Movie::IsPlaying() {
	return isPlaying;

	/*   if (pMediaCont == nullptr) {
		   return;
	   }
	   pMediaCont->GetState(0, (OAFilterState*)&filterState);*/
}


void Movie::SetSize(const D3DXVECTOR2& size) {
	if (hWnd == nullptr) {
		return;
	}

	if (size == D3DXVECTOR2(0, 0)) {
		destR = srcR;
	}
	else {
		destR.right = size.x;
		destR.bottom = size.y;
	}

	SetWindowPos(hWnd, HWND_TOP, screenPos.x, screenPos.y, destR.right, destR.bottom, SWP_SHOWWINDOW);
	pControl->SetVideoPosition(&srcR, &destR);
}
void Movie::SetPos(const D3DXVECTOR2& screenPos) {
	if (hWnd == nullptr) {
		return;
	}
	this->screenPos = screenPos;
	SetWindowPos(hWnd, HWND_TOP, this->screenPos.x, this->screenPos.y, destR.right, destR.bottom, SWP_SHOWWINDOW);
	pControl->SetVideoPosition(&srcR, &destR);
}


void Movie::SetIsLoop(bool isLoop) {
	this->isLoop = isLoop;
}

void Movie::HandleEvent() {
	long lEventCode; LONG_PTR lEvParam1, lEvParam2;

	HRESULT hr;
	do {
		//イベントを取得
		hr = pMediaEvent->GetEvent(&lEventCode, &lEvParam1, &lEvParam2, 0);
		if (FAILED(hr)) {
			break;
		}

		switch (lEventCode){
		case EC_COMPLETE:

			if (isLoop) {
				pMediaCont->Stop();

				LONGLONG llAbsoluteTime = 0;
				pMediaSeek->SetPositions(&llAbsoluteTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
				pMediaCont->Run();
			}
			else {
				isPlaying = false;
			}
			break;

		default:
			break;
		}

		pMediaEvent->FreeEventParams(lEventCode, lEvParam1, lEvParam2);

	} while (hr == S_OK);

}

LRESULT CALLBACK Movie::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_DIRECTSHOWMESSAGE:
		HandleEvent();
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK Movie::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	Movie* This = (Movie*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!This) {//取得できなかった(ウィンドウ生成中)場合
		if (message == WM_CREATE) {
			This = (Movie*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			if (This) {
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)This);
				return This->WndProc(hWnd, message, wParam, lParam);
			}
		}
	}
	else {//取得できた場合(ウィンドウ生成後)
		return This->WndProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}