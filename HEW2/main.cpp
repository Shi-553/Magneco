#define _CRT_SECURE_NO_WARNINGS

//機種依存のライブラリ指定
//#pragma comment(lib,"d3d9.lib")
//#if  defined(DEBUG)||defined(_DEBUG)
//#pragma comment(lib,"d3dx9d.lib")
//$else
//#pragma comment(lib,"d3dx9.lib")
//#endif //  defined(DEBUG)||defined(_DEBUG)


#include <Windows.h>
#include "debugPrintf.h"
#include <d3d9.h>

//数学系のライブラリ DX11とかだとまた別にリンクしないといけない
#include <d3dx9.h>

#include "config.h"
#include "myd3d.h"
#include "sprite.h"
#include "texture.h"
#include "debugFont.h"
#include "system_timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "time.h"


#define CLASS_NAME "GameWindow"
#define WINDOW_CAPTION "ウインドウ表示処理"


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


static bool Init(HWND hWnd);
static void Update(void);
static void Draw();
static void Uninit(void);


int frameCount = 0;
int baseFrame = 0;

double baseTime = 0.0;
double fps = 0.0;
double reserveTime = 0.0;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	//オリジナルのウインドウクラスを作って登録
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;                          //ウインドウプロシージャ登録
	wc.lpszClassName = CLASS_NAME;                     //オリジナルパーツのクラス名
	wc.hInstance = hInstance;                          //インスタンスハンドルを登録
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          //マウルカーソルの指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); //背景色の設定   COLOR_BACKGROUND 系は +1 しないといけない

	RegisterClass(&wc);

	DWORD windowsStyle = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&rect, windowsStyle, false);

	LONG windowWidth = rect.right - rect.left;
	LONG windowHeight = rect.bottom - rect.top;


	//1. ウインドウの幅、高さの取得色々

	//APIを利用してウインドウの幅、高さを取得
	//RECT desktopRect;
	//GetWindowRect(GetDesktopWindow(), &desktopRect);

	//int desktopWidth = desktopRect.right - desktopRect.left;
	//int desktopHeight = desktopRect.bottom - desktopRect.top;


	//2. デバイス（システム）からモニター解像度を取得
	int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
	int desktopHeight = GetSystemMetrics(SM_CYSCREEN);


	//3. デュアルモニターの真ん中（ほぼ役に立たない）
	//int virtualDesktopWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	//int virtualDesktopHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	//int virtualDesktopX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	//int virtualDesktopY = GetSystemMetrics(SM_YVIRTUALSCREEN);

	//ウインドウを真ん中にするためのXY
	int windowX = (desktopWidth - windowWidth) / 2;
	if (windowX < 0)
		windowX = 0;

	int windowY = (desktopHeight - windowHeight) / 2;
	if (windowY < 0)
		windowY = 0;


	//ウインドウ作る
	//ハンドル = ユニークなID
	HWND hWnd = CreateWindow(
		CLASS_NAME,              //ウインドウ クラス
		WINDOW_CAPTION,          //ウインドウ テキスト(上にでてくる)
		windowsStyle,     //ウインドウ スタイル

		//サイズと位置
		windowX,   //画面の左からの自身の左の座標
		windowY,   //画面の上からの自身の上の座標

		windowWidth,   //ウインドウの幅
		windowHeight,   //ウインドウの高さ (フレームを含む)

		NULL,            //親のウインドウハンドル   
		NULL,            //メニューハンドル         ↑「ファイル 編集 表示 プロジェクト...」
		hInstance,       //インスタンスハンドル
		NULL             //追加のアプリケーションデータ (ほぼ使わない)
	);

	if (hWnd == NULL) {
		//ウインドウハンドルがなんか生成できなかった
		return -1;
	}

	if (!Init(hWnd)) {
		return -1;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	//ウインドウメッセージの取得 
	MSG msg = {};
	while (TRUE) { //GetMessage内でメッセージ待ちしてる

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (WM_QUIT == msg.message)
				break;

			TranslateMessage(&msg);//なんか翻訳するんだか整えるんだか

			DispatchMessage(&msg);//これでウインドウプロシージャが呼ばれる
		}
		else {
			double time = SystemTimer_GetTime();

			if ((time - reserveTime) < 1.0f / 60.0f) {
				Sleep(0);
				continue;
			}
			reserveTime = time;

			Update();
			Draw();
		}
	}

	Uninit();
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
		//エスケープで終わるショートカット
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "終了していい？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);   //WM_QUITメッセージの送信
		return 0;
	}

	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);

		break;
	}
	Mouse_ProcessMessage(uMsg, wParam, lParam);

	//デフォルトのメッセージ処理
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


bool Init(HWND hWnd) {
	Keyboard_Initialize();
	Mouse_Initialize(hWnd);

	InitTime(SystemTimer_GetTime());

	InitInputLogger();

	if (!InitMyD3D(hWnd)) {

		MessageBox(NULL, "初期化エラー", "エラー", MB_OK);
	}
	InitTexture();

	InitSceneManager(GameStartScene);

	InitDebugFont();

	SystemTimer_Initialize();
	reserveTime = baseTime = SystemTimer_GetTime();
	frameCount = baseFrame = 0;
	fps = 0.0;


	InitSprite();

	return true;

}

void Update(void) {
	UpdateInputLogger();
	UpdateSceneManager();

	UpdateTime(SystemTimer_GetTime());

	double time = SystemTimer_GetTime();

	if (time - baseTime > 1.0) {
		fps = (frameCount - baseFrame) / (time - baseTime);

		baseTime = time;
		baseFrame = frameCount;

	}
	frameCount++;
}


void Draw() {

	LPDIRECT3DDEVICE9 d3dDevice = GetD3DDevice();
	if (d3dDevice == NULL) {
		return;
	}

	//画面のクリアBeginScene,EndSceneの外に書いて
	//第三引数：(色バッファ、深度(Z)バッファ、ステンシルバッファ)
	//第四引数：(何色でクリアするか)
	//第五引数：(Zバッファの初期化：手前 0～1 奥)
	//第六引数：(ステンシルの初期化用)
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(30, 30,80, 255), 1.0f, 0);

	//描画命令を貯めていく
	d3dDevice->BeginScene();

	DrawSceneManager();


	DEBUG_FONT_STATE state;
	state.z = -1000;
	SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	DrawDebugFont(&state,"FPS= %.2f", fps);



	//貯めるの終わり
	d3dDevice->EndScene();

	//命令を基にバッファに絵を描いて、手前の絵と入れ替える
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}



void Uninit(void) {

	UninitSprite();
	UninitSceneManager();


	UninitDebugFont();

	UninitTexture();

	UninitMyD3D();

	UninitTime();

	UninitInputLogger();
}