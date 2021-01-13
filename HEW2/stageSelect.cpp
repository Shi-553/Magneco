#include "stageSelect.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "sprite.h"
#include "texture.h"
#include "config.h"
#include "messeage.h"
#include "font.h"
#include "sceneManager.h"

#define POS_Y (-20)

#define SAMUNE_X (320)
#define SAMUNE_Y (94+POS_Y)

#define BU_LEFT_X (152)
#define BU_LEFT_Y (230+POS_Y)
#define BU_RIGHT_X (1000)
#define BU_RIGHT_Y (230+POS_Y)

#define ICON_Y (650+POS_Y)
#define ICON_TO_ICON_WIDTH (553-441)

#define MESSAGEBOX_X (SCREEN_WIDTH/2- MESSAGEBOX_WIDTH/2)
#define MESSAGEBOX_Y (SCREEN_HEIGHT / 2 +120+POS_Y)

#define MESSAGEBOX_WIDTH (700)
#define MESSAGEBOX_HEIGHT (135)

LPD3DXFONT font;

enum StageSelectTexture {
	STAGE_SELECT_BACK_GROUND,
	STAGE_SELECT_BU_L,
	STAGE_SELECT_BU_L_CANT_SELECT,
	STAGE_SELECT_BU_L_PUDHED,
	STAGE_SELECT_BU_R,
	STAGE_SELECT_BU_R_CANT_SELECT,
	STAGE_SELECT_BU_R_PUDHED,
	STAGE_SELECT_ICON,
	STAGE_SELECT_ICON_SELECT,
	STAGE_SELECT_ICON_NOT_OPEN,
	STAGE_SELECT_SAMUNE,
	STAGE_SELECT_SAMUNE_PRE,
	STAGE_SELECT_MESSAGE_BOX,
	STAGE_SELECT_MAX
};

static int selectStageTextureIds[STAGE_SELECT_MAX];

void InitStageSelect() {
	InitMesseage();

	InitSelectButton();
	selectStageTextureIds[STAGE_SELECT_BACK_GROUND] = ReserveTextureLoadFile("texture/background/背景4.jpg");
	selectStageTextureIds[STAGE_SELECT_BU_L ] =            ReserveTextureLoadFile("texture/stageSelect/button_left.png");
	selectStageTextureIds[STAGE_SELECT_BU_L_CANT_SELECT] =  ReserveTextureLoadFile("texture/stageSelect/button_left_cantselect.png");
	selectStageTextureIds[STAGE_SELECT_BU_L_PUDHED] =      ReserveTextureLoadFile("texture/stageSelect/button_left_pushed.png");
	selectStageTextureIds[STAGE_SELECT_BU_R] =             ReserveTextureLoadFile("texture/stageSelect/button_right.png");
	selectStageTextureIds[STAGE_SELECT_BU_R_CANT_SELECT ] =ReserveTextureLoadFile("texture/stageSelect/button_right_cantselect.png");
	selectStageTextureIds[STAGE_SELECT_BU_R_PUDHED] =      ReserveTextureLoadFile("texture/stageSelect/button_right_pushed.png");
	selectStageTextureIds[STAGE_SELECT_ICON] =             ReserveTextureLoadFile("texture/stageSelect/icon_noselect.png");
	selectStageTextureIds[STAGE_SELECT_ICON_SELECT ] =     ReserveTextureLoadFile("texture/stageSelect/icon_select.png");
	selectStageTextureIds[STAGE_SELECT_ICON_NOT_OPEN ] =   ReserveTextureLoadFile("texture/stageSelect/icon_notopen.png");
	selectStageTextureIds[STAGE_SELECT_SAMUNE ] =          ReserveTextureLoadFile("texture/stageSelect/samune.png");
	selectStageTextureIds[STAGE_SELECT_SAMUNE_PRE ] =      ReserveTextureLoadFile("texture/stageSelect/samune_pre.png");
	selectStageTextureIds[STAGE_SELECT_MESSAGE_BOX] =      ReserveTextureLoadFile("texture/Textbox_Test.png");
	LoadTexture();

	SetFontCountMax(52,6);
	SetFontRectAddY(140+ POS_Y);
	SetFontScale(D3DXVECTOR2(0.7, 0.7));
	SetFontMargin(7);

	MyCreateFont(32*1.2, 15 * 1.2, &font);
}
void UninitStageSelect() {
	font->Release();
	UninitSelectButton();
	ReleaseTexture(selectStageTextureIds, STAGE_SELECT_MAX);

	UninitMesseage();
}
static bool isChange = false;
void UpdateStageSelect() {

	/*if (TriggerInputLogger(MYVK_ENTER)) {
		TriggerSelectButton();
		isChange = false;
	}
	if (!isChange && ReleaseInputLogger(MYVK_ENTER)) {
		ReleaseSelectButton();
	}
	if (TriggerInputLogger(MYVK_LEFT)) {
		ForwardSelectButton();
		startButton.textureId = backGround;
		endButton.textureId = endTexture;
		isChange = true;

	}*/
	if (TriggerInputLogger(MYVK_ENTER)) {
		GoNextScene(GameScene);
	}

}
void DrawStageSelect() {
	DrawSelectButton();
	DrawSprite(selectStageTextureIds[STAGE_SELECT_BACK_GROUND], D3DXVECTOR2(0, 0), 1);

	DrawSprite(selectStageTextureIds[STAGE_SELECT_BU_L_CANT_SELECT], D3DXVECTOR2(BU_LEFT_X, BU_LEFT_Y), 1);
	DrawSprite(selectStageTextureIds[STAGE_SELECT_BU_R_CANT_SELECT], D3DXVECTOR2(BU_RIGHT_X, BU_RIGHT_Y), 1);

	DrawSprite(selectStageTextureIds[STAGE_SELECT_SAMUNE_PRE], D3DXVECTOR2(SAMUNE_X, SAMUNE_Y), 1);

	DrawSprite(selectStageTextureIds[STAGE_SELECT_SAMUNE], D3DXVECTOR2(SAMUNE_X, SAMUNE_Y), 1);
	RECT r;
	r.top = SAMUNE_Y+40;
	r.left = SAMUNE_X+20;
	r.right =0;
	r.bottom = 0;
	font->DrawTextA(NULL, "ステージ１", -1, &r, DT_NOCLIP, D3DCOLOR_RGBA(255, 255, 255, 255));

	DrawSprite(selectStageTextureIds[STAGE_SELECT_MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });


	ClearMesseageOffset();
	DrawMesseage("はりぼて");

	DrawSprite(selectStageTextureIds[STAGE_SELECT_ICON_SELECT], D3DXVECTOR2(SCREEN_WIDTH / 2, ICON_Y), 1);

}