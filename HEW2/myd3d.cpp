//Direct3Dの核を初期化
// a
#include <Windows.h>


#include "myd3d.h"
#include "config.h"


static LPDIRECT3D9 d3d = NULL;
static LPDIRECT3DDEVICE9 d3dDevice = NULL;


//戻り値 失敗:false
bool InitMyD3D(HWND hWnd) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) {
		MessageBox(NULL, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//スワップ方法
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;//深度バッファ・ステンシルバッファの使用1
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//深度バッファ・ステンシルバッファのフォーマット

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//フルスクリーンのときのリフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//後で

	HRESULT hr;
	hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);

	if (FAILED(hr)) {
		MessageBox(NULL, "Direct3Dデバイスの取得に失敗しました", "エラー", MB_OK);
		return false;
	}


	//テクスチャフィルタリングの設定
	//拡大縮小時にする補間と、それにミップマップを使うかどうか
	//gPDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);//拡大時フィルタ
	//gPDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);//縮小時フィルタ
	//gPDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);//ミップマップフィルタ

	d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	//アドレッシングモードの設定
	//テクスチャのアルファ値を使える
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//アルファブレンドON
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//ソースにソースのアルファをかける
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//ソースの下にあるものは（１－ソースのアルファ）をかける
	d3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);//


	//デフォルトのテクスチャステージステート
	//色をどうするかシェーダー使うと要らなくなる
	//RGB
	//gPDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_CURRENT);
	//gPDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//gPDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	//gPDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//OP(演算子)SELECTARG1 テクスチャだけ SELECTARG2 頂点カラーだけ

	//ALPHA
	//gPDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_CURRENT);
	//gPDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//gPDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	//gPDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//乗算になる
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return true;
}

void UninitMyD3D() {
	if (d3dDevice) {
		d3dDevice->Release();
		d3dDevice = NULL;
	}
	if (d3d) {
		d3d->Release();
		d3d = NULL;
	}

}

LPDIRECT3DDEVICE9 GetD3DDevice() {
	return d3dDevice;
}