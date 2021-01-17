
#include "messeage.h"
#include "tutorial.h"
#include "font.h"
#include "trans.h"

#include "InputLogger.h"

#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "gameSrite.h"
#include "debugFont.h"
#include "player.h"
#include "npc.h"
#include "gameSrite.h"
#include "game.h"

#include "Input.h"
#include "judge.h"

#include "map.h"
#include "flyingObjectSponer.h"
#include "debugPrintf.h"

#include "messeagebox.h"
#include "highlight.h"
#include "enterBottonSprite.h"
#include "sceneManager.h"

#define MOVE_TEXTURE_WIDTH 32
#define MOVE_TEXTURE_HEIGHT 32


#define GREAT_TEXTURE_WIDTH 384
#define GREAT_TEXTURE_HIGHT 128

#define GREAT_TEXTURE_SIZE_WIDTH 500
#define GREAT_TEXTURE_SIZE_HIGHT 160

#define TUTORIAL_FONT_WIDTH 15
#define TUTPRIAL_FONT_HIGHT 32

static int WBottonTexture = TEXTURE_INVALID_ID;
static int ABottonTexture = TEXTURE_INVALID_ID;
static int SBottonTexture = TEXTURE_INVALID_ID;
static int DBottonTexture = TEXTURE_INVALID_ID;
static int GreatTexture = TEXTURE_INVALID_ID;

static int frame = 0;
static int aniFrame = 0;

static LPD3DXSPRITE sprite;
static INTVECTOR2 offset;

static bool WPress = false;
static bool SPress = false;
static bool APress = false;
static bool DPress = false;

static Player player;

static Message* message;
void InitTutorial()
{
	WBottonTexture = ReserveTextureLoadFile("texture/move_W.png");
	ABottonTexture = ReserveTextureLoadFile("texture/move_A.png");
	SBottonTexture = ReserveTextureLoadFile("texture/move_S.png");
	DBottonTexture = ReserveTextureLoadFile("texture/move_D.png");
	GreatTexture = ReserveTextureLoadFile("texture/greattext384_128_01.png");

	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	D3DXCreateSprite(device, &sprite);

	WPress = false;
	SPress = false;
	APress = false;
	DPress = false;

	InitPlayer();
	InitInput();
	InitNPC();
	InitMap();
	InitJudge();
	InitFlyingObject();
	InitFlyingSponer();
	InitEnterBottonSprite();
	InitFont();
	InitMesseageBox();
	//Grid_Initialize(GAME_SPRITE_WHIDTH, GetMapWidth(), D3DCOLOR_RGBA(0, 197, 0, 255));

	aniFrame = 0;
	frame = 1;
	message = new Message();

	LoadTexture();
}

void UninitTutorial()
{
	UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitJudge();
	UninitMap();

	UninitFlyingObject();
	UninitFlyingSponer();

	UninitEnterBottonSprite();
	UninitFont();
	UninitMesseageBox();

	ReleaseTexture(WBottonTexture);
	ReleaseTexture(ABottonTexture);
	ReleaseTexture(SBottonTexture);
	ReleaseTexture(DBottonTexture);
	ReleaseTexture(GreatTexture);

	delete message;
}

void DrawTutorial()
{
	DrawMap();


	DrawInput();


	DrawJudge();
	//DrawFlyingObject();

	DrawPlayer();

	if (frame <= 7 || frame >= 38) {
		DrawNPC();;
	}


	message->ClearOffset();

	message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//----------------------------------------------------------------------------
	// ゲーム概要
	//----------------------------------------------------------------------------

	if (frame >= 1 && frame <= 7) {

		if (frame == 1) {
			DrawMesseageBox();


			message->Draw("チュートリアルステージへようこそ！\n\nここではマグネッコでの操作やルールに\n\nついて説明していきます！");

			DrawEnterBottonSprite();
		}
		if (frame == 2) {
			DrawMesseageBox();
			message->Draw("まずはゲームをクリアするために\n\n必要なルールとオブジェクトを\n\n確認しましょう！");

			DrawEnterBottonSprite();
		}
		if (frame == 3) {
			DrawMesseageBox();

			message->Draw("このゲームは");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("プレイヤーであるマグネッコを\n\n動かし、NPCキャラであるローズを\n\nゴールまで導くことでクリア");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("となります");

			DrawEnterBottonSprite();

		}
		if (frame == 4) {

			sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			// プレイヤー
			Player* player = GetPlayer();

			auto hlightplayer = player->trans.pos;

			DrawHighlight(GameToScreenPos(hlightplayer), D3DXVECTOR2(50, 50), D3DXVECTOR2(0.5f, 0.5f), 100, 0.0f);

			offset = INTVECTOR2(0, 0);



			/*RECT rect = {
		,	         	// 左上のx座標
		,				// 左上のy座標
		,		                                    // 右下のx座標
		 // 右下のy座標
			};

			font->DrawTextA(sprite, "<マグネッコ(プレイヤー)", -1, rect, DT_LEFT, D3DXCOLOR(255, 255, 2555, 255));*/

			// NPC
			NPC* npc = GetNpc();
			auto hlightnpc = npc->trans.pos;

			DrawHighlight(GameToScreenPos(hlightnpc), D3DXVECTOR2(50, 50), D3DXVECTOR2(0.25f, 0.45f), 100, 0.0f);

			// ゴール
			auto hlightgoal = D3DXVECTOR2(
				8,
				3
			);

			DrawHighlight(GameToScreenPos(hlightgoal), D3DXVECTOR2(50, 50), D3DXVECTOR2(0.75f, 0.8f), 100, 0.0f);

			sprite->End();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();

		}
		if (frame == 5) {
			DrawMesseageBox();

			message->Draw("しかし今の状態では道がないため\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ゴールまでローズが辿り着けません・・・");

			DrawEnterBottonSprite();
		}
		if (frame == 6) {

			SetBackStencilBufferMask();

			for (int i = 3; i < 8; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					7
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}
			for (int i = 3; i < 7; i++) {

				auto hlight = D3DXVECTOR2(
					7,
					i
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 7) {
			DrawMesseageBox();

			message->Draw("ローズがゴールまで辿り着けるように\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("基礎動作とルートの作り方");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("を覚えましょう！");

			DrawEnterBottonSprite();
		}
	}

	//----------------------------------------------------------------------------
	// 移動チュートリアル
	//----------------------------------------------------------------------------

	if (frame >= 8 && frame <= 18) {
		if (frame == 8) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 140, 0, 255));
			message->Draw("\n\n       次は移動チュートリアルです");

			DrawEnterBottonSprite();
		}
		if (frame == 9) {
			DrawMesseageBox();

			message->Draw("最初にこのゲームをプレイしていく上で\n\n必要不可欠となる");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("移動");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("からやっていきましょう");

			DrawEnterBottonSprite();
		}
		if (frame == 10) {
			DrawMesseageBox();
			message->Draw("移動はWASDキーで行えます\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("W=上　S=下　A=左　D=右");

			DrawEnterBottonSprite();
		}
		if (frame == 11) {
			DrawMesseageBox();

			message->Draw("表示されるガイドに従って移動してみましょう！");

			DrawEnterBottonSprite();
		}
		if (frame == 12) {

			// Wキー
			if (WPress == false) {

				Player* player = GetPlayer();
				auto drawingPos = player->trans.pos;

				drawingPos.y -= 1;

				auto tPos = D3DXVECTOR2(
					MOVE_TEXTURE_WIDTH * (aniFrame / 5 % 5),
					0
				);

				DrawGameSprite(WBottonTexture, drawingPos - D3DXVECTOR2(0.5, 0.5), 30, tPos, D3DXVECTOR2(MOVE_TEXTURE_WIDTH, MOVE_TEXTURE_HEIGHT));
			}
			if (ReleaseInputLogger(MYVK_UP)) {
				WPress = true;
			}

			// Sキー
			if (SPress == false) {

				Player* player = GetPlayer();
				auto drawingPos = player->trans.pos;

				drawingPos.y += 1;

				auto tPos = D3DXVECTOR2(
					MOVE_TEXTURE_WIDTH * (aniFrame / 5 % 5),
					0
				);

				DrawGameSprite(SBottonTexture, drawingPos - D3DXVECTOR2(0.5, 0.5), 30, tPos, D3DXVECTOR2(MOVE_TEXTURE_WIDTH, MOVE_TEXTURE_HEIGHT));

			}
			if (ReleaseInputLogger(MYVK_DOWN)) {
				SPress = true;

			}

			// Aキー
			if (APress == false) {

				Player* player = GetPlayer();
				auto drawingPos = player->trans.pos;

				drawingPos.x -= 1;

				auto tPos = D3DXVECTOR2(
					MOVE_TEXTURE_WIDTH * (aniFrame / 5 % 5),
					0
				);

				DrawGameSprite(ABottonTexture, drawingPos - D3DXVECTOR2(0.5, 0.5), 30, tPos, D3DXVECTOR2(MOVE_TEXTURE_WIDTH, MOVE_TEXTURE_HEIGHT));
			}
			if (ReleaseInputLogger(MYVK_LEFT)) {
				APress = true;

			}

			// Dキー
			if (DPress == false) {

				Player* player = GetPlayer();
				auto drawingPos = player->trans.pos;

				drawingPos.x += 1;

				auto tPos = D3DXVECTOR2(
					MOVE_TEXTURE_WIDTH * (aniFrame / 5 % 5),
					0
				);

				DrawGameSprite(DBottonTexture, drawingPos - D3DXVECTOR2(0.5, 0.5), 30, tPos, D3DXVECTOR2(MOVE_TEXTURE_WIDTH, MOVE_TEXTURE_HEIGHT));

			}
			if (ReleaseInputLogger(MYVK_RIGHT)) {
				DPress = true;

			}

		}
		if (frame == 13) {
			DrawSprite(GreatTexture, { 400,250 }, 10, { GREAT_TEXTURE_SIZE_WIDTH,GREAT_TEXTURE_SIZE_HIGHT }, { 0,0 }, { GREAT_TEXTURE_WIDTH,GREAT_TEXTURE_HIGHT });

			DrawEnterBottonSprite();
		}
		if (frame == 14) {
			DrawMesseageBox();

			message->Draw("上手に移動できたみたいですね！\n\n移動は最もよく使う動作なのでスピード感を\n\nしっかり把握しておきましょう！");

			DrawEnterBottonSprite();
		}
		if (frame == 15) {
			DrawMesseageBox();

			message->Draw("さらに");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("W+Dで右上、W+Aで左上、\n\n S+Dで右下、S+Aで左下と斜め移動");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("できます\n\n使えると操作の幅が広がりますよ！");

			DrawEnterBottonSprite();
		}
		if (frame == 16) {
			DrawMesseageBox();

			message->Draw("また");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("赤いブロックのある場所から外側");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("は\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("エリア外");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("となり出ることができないので\n\n注意してください");

			DrawEnterBottonSprite();
		}
		if (frame == 17) {
			SetBackStencilBufferMask();

			for (int i = 0; i < 10; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					0
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}
			for (int i = 0; i < 10; i++) {

				auto hlight = D3DXVECTOR2(
					0,
					i
				);
				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}
			for (int i = 0; i < 10; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					9
				);
				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}
			for (int i = 0; i < 10; i++) {

				auto hlight = D3DXVECTOR2(
					9,
					i
				);
				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();

		}
		if (frame == 18) {
			DrawMesseageBox();

			message->Draw("では次はルート作成について解説していきます！");

			DrawEnterBottonSprite();
		}

	}

	//----------------------------------------------------------------------------
	// ブロック入手チュートリアル
	//----------------------------------------------------------------------------

	if (frame >= 19 && frame <= 37) {

		if (frame == 19) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 140, 0, 255));
			message->Draw("\n\n       次はブロック入手チュートリアルです");

			DrawEnterBottonSprite();
		}
		if (frame == 20) {
			DrawMesseageBox();

			message->Draw("NPCのルートを作っていく上で必要になるのが\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ブロック");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("です。まずブロックがどんなものか\n\n見てみましょう");

			DrawEnterBottonSprite();
		}
		if (frame == 21) {

			SetBackStencilBufferMask();

			for (int i = 0; i < 13; i++) {

				auto hlight = D3DXVECTOR2(
					2,
					i
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 22) {

			SetBackStencilBufferMask();

			for (int i = 0; i < 13; i++) {

				auto hlight = D3DXVECTOR2(
					4,
					i
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 23) {

			SetBackStencilBufferMask();

			for (int i = 0; i < 13; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					2
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 24) {

			SetBackStencilBufferMask();

			for (int i = 0; i < 13; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					4
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 25) {
			DrawMesseageBox();

			message->Draw("先ほど飛んできたのが\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ブロック");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("です\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("4方向からエリア内に向けて");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("飛んできます");

			DrawEnterBottonSprite();
		}
		if (frame == 26) {
			DrawMesseageBox();

			message->Draw("マグネッコは");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ブロックにぶつかる");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("ることで\n\n強力な磁力を発揮し");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ぶつけた部分に\n\nブロックをくっつけられます！");

			DrawEnterBottonSprite();
		}
		if (frame == 27) {
			DrawMesseageBox();

			message->Draw("臆せず移動してブロックにぶつかって\n\nみましょう！");

			DrawEnterBottonSprite();
		}
		if (frame == 28) {

			DrawFlyingObject();

			DrawEnterBottonSprite();
		}
		if (frame == 29) {

			DrawSprite(GreatTexture, { 400,250 }, 10, { GREAT_TEXTURE_SIZE_WIDTH,GREAT_TEXTURE_SIZE_HIGHT }, { 0,0 }, { GREAT_TEXTURE_WIDTH,GREAT_TEXTURE_HIGHT });

			DrawEnterBottonSprite();
		}
		if (frame == 30) {

			DrawMesseageBox();

			message->Draw("ブロックをくっつけられましたね！\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("くっついたブロックには更にブロックを\n\nくっつけられます！");


			DrawEnterBottonSprite();
		}
		if (frame == 31) {

			DrawMesseageBox();

			message->Draw("ただしマグネッコの磁力にも限界があり\n\nくっつけられるブロックは");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("最大で4つまで\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("となっています");


			DrawEnterBottonSprite();
		}
		if (frame == 32) {

			DrawMesseageBox();

			message->Draw("以上のことをふまえて最大までブロックを\n\nくっつけてみましょう！");

			DrawEnterBottonSprite();
		}
		if (frame == 33) {

			DrawFlyingObject();

			DrawEnterBottonSprite();
		}
		if (frame == 34) {

			DrawSprite(GreatTexture, { 400,250 }, 10, { GREAT_TEXTURE_SIZE_WIDTH,GREAT_TEXTURE_SIZE_HIGHT }, { 0,0 }, { GREAT_TEXTURE_WIDTH,GREAT_TEXTURE_HIGHT });

			DrawEnterBottonSprite();
		}
		if (frame == 35) {

			DrawMesseageBox();

			message->Draw("ブロックのくっつけ方についてはお分かり\n\nいただけたでしょうか？");

			DrawEnterBottonSprite();
		}
		if (frame == 36) {

			DrawMesseageBox();

			message->Draw("またもう既に体験したかもしれませんが\n\nブロックに");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("斜め移動でぶつかるとブロックも\n\n斜めの方向にくっつきます");


			DrawEnterBottonSprite();
		}
		if (frame == 37) {

			DrawMesseageBox();

			message->Draw("ではこのくっつけたブロックをどうすれば\n\nNPCを導くルートにできるのか試してみましょう");

			DrawEnterBottonSprite();
		}
	}


	//----------------------------------------------------------------------------
	// ブロック設置チュートリアル
	//----------------------------------------------------------------------------

	if (frame >= 38 && frame <= 48) {
		if (frame == 38) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 140, 0, 255));
			message->Draw("\n\n       次はブロック設置チュートリアルです");

			DrawEnterBottonSprite();
		}
		if (frame == 39) {

			DrawMesseageBox();

			message->Draw("くっつけたブロックは");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ローズか、作った\n\nルートと隣接した場所");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("に置くことができます\n\n今の状態だとこの場所です");


			DrawEnterBottonSprite();
		}
		if (frame == 40) {

			SetBackStencilBufferMask();

			for (int i = 1; i < 4; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					7
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}
			for (int i = 6; i < 9; i++) {

				auto hlight = D3DXVECTOR2(
					2,
					i
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 41) {
			DrawMesseageBox();

			message->Draw("先ほど示した場所にブロックを一つ置いて\n\nルートにしてみましょう");

			DrawEnterBottonSprite();
		}
		if (frame == 42) {

			SetBackStencilBufferMask();

			for (int i = 1; i < 4; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					7
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}
			for (int i = 6; i < 9; i++) {

				auto hlight = D3DXVECTOR2(
					2,
					i
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 43) {

			DrawSprite(GreatTexture, { 400,250 }, 10, { GREAT_TEXTURE_SIZE_WIDTH,GREAT_TEXTURE_SIZE_HIGHT }, { 0,0 }, { GREAT_TEXTURE_WIDTH,GREAT_TEXTURE_HIGHT });

			DrawEnterBottonSprite();
		}
		if (frame == 44) {

			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ブロックの色が変化し、ルートになりました！\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("次は複数ブロックをくっつけた状態で\n\nルートを作ってみましょう");

			DrawEnterBottonSprite();
		}
		if (frame == 45) {

			SetBackStencilBufferMask();

			for (int i = 1; i < 4; i++) {

				auto hlight = D3DXVECTOR2(
					i,
					7
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}
			for (int i = 6; i < 9; i++) {

				auto hlight = D3DXVECTOR2(
					2,
					i
				);

				DrawGameSprite(TEXTURE_INVALID_ID, hlight, 100);
			}

			ResetBackStencilBufferMask();

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 46) {

			DrawSprite(GreatTexture, { 400,250 }, 10, { GREAT_TEXTURE_SIZE_WIDTH,GREAT_TEXTURE_SIZE_HIGHT }, { 0,0 }, { GREAT_TEXTURE_WIDTH,GREAT_TEXTURE_HIGHT });

			DrawEnterBottonSprite();
		}
		if (frame == 47) {
			DrawMesseageBox();

			message->Draw("複数のブロックもちゃんとルートにできた\n\nみたいですね！これでローズをゴールまで\n\n導くルートができそうですが・・・");

			DrawEnterBottonSprite();
		}
		if (frame == 48) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ローズまたはルートと隣接していても\n\nブロックを置けない");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("場合があります");

			DrawEnterBottonSprite();
		}
	}

	//----------------------------------------------------------------------------
	// 設置不可チュートリアル
	//----------------------------------------------------------------------------

	if (frame >= 49 && frame <= 61) {

		if (frame == 49) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 140, 0, 255));
			message->Draw("\n\n       次は設置不可チュートリアルです");

			DrawEnterBottonSprite();
		}
		if (frame == 50) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ブロックを置けない状況は大きく分けて\n\n四つ");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("存在します");

			DrawEnterBottonSprite();
		}
		if (frame == 51) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("◯状況その1\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("ブロックを置こうとしている位置がローズ、\n\nルートに隣接していない");

			DrawEnterBottonSprite();
		}
		if (frame == 52) {

			D3DXVECTOR2 highlight(
				4,
				7
			);

			DrawHighlight(GameToScreenPos(highlight), D3DXVECTOR2(185, 125), D3DXVECTOR2(0.5f, 0.5f), 100, 0.0f);

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 53) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("◯状況その2\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("ブロックを置こうとしている位置がローズ\n\nルートと重なっている");

			DrawEnterBottonSprite();
		}
		if (frame == 54) {

			D3DXVECTOR2 highlight(
				3,
				7
			);

			DrawHighlight(GameToScreenPos(highlight), D3DXVECTOR2(125, 75), D3DXVECTOR2(0.5f, 0.5f), 100, 0.0f);

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 55) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("◯状況その3\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("ブロックを置こうとしている位置がエリア内外を\n\n分けるブロックと重なっている");

			DrawEnterBottonSprite();
		}
		if (frame == 56) {

			D3DXVECTOR2 highlight(
				2,
				8
			);

			DrawHighlight(GameToScreenPos(highlight), D3DXVECTOR2(175, 125), D3DXVECTOR2(0.5f, 0.5f), 100, 0.0f);

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 57) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("◯状況その4\n\n");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("ブロックを置こうとしている位置が障害物\n\nブロックと重なっている");

			DrawEnterBottonSprite();
		}
		if (frame == 58) {

			D3DXVECTOR2 highlight(
				5,
				6
			);

			DrawHighlight(GameToScreenPos(highlight), D3DXVECTOR2(50, 50), D3DXVECTOR2(0.75f, 0.75f), 100, 0.0f);

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 59) {

			D3DXVECTOR2 highlight(
				4,
				6
			);

			DrawHighlight(GameToScreenPos(highlight), D3DXVECTOR2(175, 125), D3DXVECTOR2(0.5f, 0.5f), 100, 0.0f);

			DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

			DrawEnterBottonSprite();
		}
		if (frame == 60) {
			DrawMesseageBox();

			message->Draw("以上がブロックを置けない状況になります");

			DrawEnterBottonSprite();
		}
		if (frame == 61) {
			DrawMesseageBox();

			message->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			message->Draw("ブロックを分割して置いたりはできない");

			message->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			message->Draw("ので\n\n置けない場所に重ならないよう、上手に\n\nブロックを組みましょう");

			DrawEnterBottonSprite();
		}
	}
}

void UpdateTutorial()
{
	UpdateMap();
	UpdatePlayer();

	UpdateNPC();

	UpdateMesseageBox();
	UpdateEnterBottonSprite();

	if (frame >= 1 && frame <= 11 || frame >= 13 && frame <= 27 || frame >= 29 && frame <= 32 || frame >= 34 && frame <= 128) {
		if (ReleaseInputLogger(MYVK_ENTER)) {
			frame++;
			return;
		}
	}

	if (frame >= 12) {
		UpdateInput();
	}
	if (frame == 12) {
		if (WPress == true && SPress == true && APress == true && DPress == true) {
			frame++;
			return;
		}
	}

	if (frame == 28 || frame == 33) {

		UpdateJudge();
		UpdateFlyingObject();
		UpdateFlyingSponer();

		if (ReleaseInputLogger(MYVK_ENTER)) {
			frame++;
			return;
		}

	}

	if (frame == 62) {
		GoNextScene(GameStartScene);
	}

	aniFrame++;



	/*UpdateJudge();
	UpdateFlyingObject();
	UpdateFlyingSponer();*/

}
