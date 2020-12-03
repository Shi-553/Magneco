
#include "messeage.h"
#include "tutorial.h"

#include "InputLogger.h"

#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "debugFont.h"
#include "player.h"
#include "npc.h"
#include "gameSrite.h"
#include "game.h"

#include "Input.h"
#include "judge.h"

#include "map.h"
#include "flyingObjectSponer.h"
#include "grid.h"
#include "debugPrintf.h"

#include "messeagebox.h"
#include "highlight.h"
#include "enterBottonSprite.h"

//#define MOVE_TEXTURE_WIDTH
//#define MOVE_TEXTURE_HEIGHT

static int WBottonTexture = TEXTURE_INVALID_ID;
static int ABottonTexture = TEXTURE_INVALID_ID;
static int SBottonTexture = TEXTURE_INVALID_ID;
static int DBottonTexture = TEXTURE_INVALID_ID;

static int frame = 0;



void InitTutorial()
{
	WBottonTexture = ReserveTextureLoadFile("texture/move_W.png");
	ABottonTexture = ReserveTextureLoadFile("texture/move_A.png");
	SBottonTexture = ReserveTextureLoadFile("texture/move_S.png");
	DBottonTexture = ReserveTextureLoadFile("texture/move_D.png");

	InitPlayer();
	InitInput();
	InitNPC();
	InitMap();
	InitJudge();
	InitFlyingObject();
	InitFlyingSponer();
	InitEnterBottonSprite();
	InitMessage();
	InitMesseageBox();
	Grid_Initialize(GAME_SPRITE_WHIDTH, MAPCHIP_WIDTH, D3DCOLOR_RGBA(0, 197, 0, 255));

	frame = 1;

	LoadTexture();
}

void UninitTutorial()
{
	UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitJudge();
	Grid_Finalize();
	UninitMap();

	UninitFlyingObject();
	UninitFlyingSponer();

	UninitEnterBottonSprite();
	UninitMessage();
	UninitMesseageBox();
}

void DrawTutorial()
{
	DrawMap();
	Grid_Draw();
	//DrawNPC();


	DrawInput();


	DrawJudge();
	//DrawFlyingObject();

	DrawPlayer();

	//DrawHighlight(D3DXVECTOR2(300, 100), 100, 6);
	//DrawForeground(D3DCOLOR_RGBA(0, 0, 0, 200));

	

	ClearMessageOffset();

	SetMessageColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	if (frame == 1) {
		DrawMesseageBox();

		
		DrawMessage("チュートリアルステージへようこそ！ここではマグネッコでの操作やルールに\n\nついて説明していきます！");

		DrawEnterBottonSprite();
	}
	if (frame == 2) {
		DrawMesseageBox();
		DrawMessage("まずはゲームをクリアするために\n\n必要なルールとオブジェクトを\n\n確認しましょう！");

		DrawEnterBottonSprite();
	}
	if (frame == 3) {
		DrawMesseageBox();

		DrawMessage("このゲームは");

		SetMessageColor(D3DCOLOR_RGBA(255, 255, 0, 255));
		DrawMessage("このゲームは");

		DrawEnterBottonSprite();
	}
	if (frame == 4) {

	}
}

void UpdateTutorial()
{
	UpdateMap();
	UpdatePlayer();

	UpdateMesseageBox();
	UpdateEnterBottonSprite();

	if (frame >= 1 && frame <= 4) {
		if (TriggerInputLogger(MYVK_ENTER)) {
			frame++;
			return;
		}
	}

	UpdateInput();
	/*UpdateNPC();

	UpdateJudge();
	UpdateFlyingObject();
	UpdateFlyingSponer();*/

}
