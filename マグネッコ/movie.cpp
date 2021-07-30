#include "movie.h"
#include <Vmr9.h>
#include "config.h"



void Movie::Init() {
    DWORD windowsStyle = WS_CHILD ^ WS_EX_NOPARENTNOTIFY;

    LONG windowWidth = destR.right - destR.left;
    LONG windowHeight = destR.bottom - destR.top;



    //ウインドウ作る
    //ハンドル = ユニークなID
    hWnd = CreateWindow(
       "GameWindow",              //ウインドウ クラス
       "",          //ウインドウ テキスト(上にでてくる)
       windowsStyle,     //ウインドウ スタイル

       //サイズと位置
       screenPos.x,   //画面の左からの自身の左の座標
       screenPos.y,   //画面の上からの自身の上の座標

       windowWidth,   //ウインドウの幅
       windowHeight,   //ウインドウの高さ (フレームを含む)

       GetActiveWindow(),            //親のウインドウハンドル   
       NULL,            //メニューハンドル         ↑「ファイル 編集 表示 プロジェクト...」
       GetModuleHandle(nullptr),       //インスタンスハンドル
       NULL             //追加のアプリケーションデータ (ほぼ使わない)
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

    if (destR.right==0&&destR.bottom==0) {
        SetSize();
    }


    pGB->QueryInterface(IID_IMediaControl, (void**)&pMediaCont);

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
    if (pMediaCont == nullptr) {
        return false;
    }
    FILTER_STATE  s;
    pMediaCont->GetState(0, (OAFilterState*)&s);
    return s == FILTER_STATE::State_Running;
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