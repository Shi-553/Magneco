#include "stageSelect.h"
#include "InputLogger.h"
#include "selectButton.h"
#include "sprite.h"
#include "texture.h"
#include "config.h"
#include "messeage.h"
#include "font.h"
#include "sceneManager.h"
#include <vector>
#include "stageInfo.h"
#include "importExport.h"
#include <algorithm>
using namespace std;

#define POS_Y (-20)

#define SAMUNE_X (320)
#define SAMUNE_Y (94+POS_Y)

#define NAME_X (SAMUNE_X + 20)
#define NAME_Y (SAMUNE_Y + 30)

#define LEVEL_RX (930)
#define LEVEL_Y (SAMUNE_Y+30)

#define BU_LEFT_X (152)
#define BU_LEFT_Y (230+POS_Y)
#define BU_RIGHT_X (1000)
#define BU_RIGHT_Y (230+POS_Y)

#define ICON_Y (650+POS_Y)
#define ICON_TO_ICON_WIDTH (553-441)

#define MESSAGEBOX_X (SCREEN_WIDTH/2- MESSAGEBOX_WIDTH/2)
#define MESSAGEBOX_Y (SCREEN_HEIGHT / 2 +120+POS_Y)

#define MESSAGE_X (MESSAGEBOX_X+30)
#define MESSAGE_Y (MESSAGEBOX_Y+20)

#define MESSAGEBOX_WIDTH (700)
#define MESSAGEBOX_HEIGHT (135)

static Message* nameMessage, *overviewMessage, *lavelMessage;

static vector<StageInfo> infos;
static vector<int> smunes;

enum StageSelectTexture {
	STAGE_SELECT_BACK_GROUND,
	STAGE_SELECT_SAMUNE,
	STAGE_SELECT_SAMUNE_PRE,
	STAGE_SELECT_MESSAGE_BOX,
	STAGE_SELECT_MAX
};

static int selectStageTextureIds[STAGE_SELECT_MAX];

void InitStageSelect() {

	InitSelectButton();
	selectStageTextureIds[STAGE_SELECT_BACK_GROUND] = ReserveTextureLoadFile("texture/背景4.jpg");

	SetSelectButtonBack(ReserveTextureLoadFile("texture/stageSelect/button_left.png"),
	 ReserveTextureLoadFile("texture/stageSelect/button_left_pushed.png"),
	 ReserveTextureLoadFile("texture/stageSelect/button_left_cantselect.png"),
		D3DXVECTOR2(BU_LEFT_X, BU_LEFT_Y));

	SetSelectButtonForward(ReserveTextureLoadFile("texture/stageSelect/button_right.png"),
	ReserveTextureLoadFile("texture/stageSelect/button_right_pushed.png"),
	ReserveTextureLoadFile("texture/stageSelect/button_right_cantselect.png"),
		D3DXVECTOR2(BU_RIGHT_X, BU_RIGHT_Y));

	//selectStageTextureIds[STAGE_SELECT_ICON_NOT_OPEN ] =   ReserveTextureLoadFile("texture/stageSelect/icon_notopen.png");

	selectStageTextureIds[STAGE_SELECT_SAMUNE] = ReserveTextureLoadFile("texture/stageSelect/samune.png");
	selectStageTextureIds[STAGE_SELECT_SAMUNE_PRE] = ReserveTextureLoadFile("texture/stageSelect/samune_pre.png");
	selectStageTextureIds[STAGE_SELECT_MESSAGE_BOX] = ReserveTextureLoadFile("texture/Textbox_Test.png");

	overviewMessage = new Message();
	nameMessage = new Message();
	lavelMessage = new Message();

	overviewMessage->SetScale(D3DXVECTOR2(0.7, 0.7));
	overviewMessage->SetMargin(7);
	overviewMessage->SetPos(D3DXVECTOR2(MESSAGE_X, MESSAGE_Y));

	nameMessage->SetPos(D3DXVECTOR2(NAME_X, NAME_Y));
	nameMessage->SetScale({1.2, 1.2});

	lavelMessage->SetEndPos(D3DXVECTOR2(LEVEL_RX, -1));

	lavelMessage->SetPos(D3DXVECTOR2(-1, LEVEL_Y));
	lavelMessage->SetScale({ 1.2, 1.2 });
	lavelMessage->SetFormat(DT_RIGHT);

	GetStageInfos("stage", infos);

	std::sort(infos.begin(), infos.end(), [](StageInfo a, StageInfo b) {
		return a.index < b.index;
  });

	Button b;
	b.releasedCallback = []() {
		SetStagePath(infos[GetSelectButtonIndex()].filename);
		GoNextScene(GameScene);
	};
	b.textureId = ReserveTextureLoadFile("texture/stageSelect/icon_noselect.png");
	b.pressedTextureId = b.textureId;
	AddTextureReferenceCount(b.textureId);

	for (auto itr = infos.begin(); itr != infos.end(); itr++) {
		auto index = std::distance(infos.begin(), itr);

		b.pos = D3DXVECTOR2(SCREEN_WIDTH / 2 + (index - infos.size() / 2) * ICON_TO_ICON_WIDTH, ICON_Y);

		AddTextureReferenceCount(b.textureId);
		AddTextureReferenceCount(b.textureId);
		AddSelectButton(b);

		smunes.push_back(ReserveTextureLoadFile(itr->samunename.c_str()));
	}


	SetSelectButtonFrame(ReserveTextureLoadFile("texture/stageSelect/icon_select.png"));


	SetSelectButtonKey(MYVK_ENTER, MYVK_RIGHT, MYVK_LEFT);

	LoadTexture();
}
void UninitStageSelect() {
	UninitSelectButton();
	ReleaseTexture(selectStageTextureIds, STAGE_SELECT_MAX);


	for (auto itr = smunes.begin(); itr != smunes.end(); itr++) {
		ReleaseTexture(*itr);
	}
	smunes.clear();
	infos.clear();
	delete overviewMessage;
	delete nameMessage    ;
	delete lavelMessage   ;
}

void UpdateStageSelect() {
	UpdateSelectButton();
}
void DrawStageSelect() {
	DrawSprite(selectStageTextureIds[STAGE_SELECT_BACK_GROUND], D3DXVECTOR2(0, 0), 1);

	auto& info = infos[GetSelectButtonIndex()];
	auto& samune = smunes[GetSelectButtonIndex()];

	DrawSprite(samune, D3DXVECTOR2(SAMUNE_X, SAMUNE_Y), 1);

	DrawSprite(selectStageTextureIds[STAGE_SELECT_SAMUNE], D3DXVECTOR2(SAMUNE_X, SAMUNE_Y), 1);

	nameMessage->ClearOffset();
	nameMessage->Draw(info.name.c_str());

	DrawSprite(selectStageTextureIds[STAGE_SELECT_MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });


	if (!info.overview.empty()) {
		overviewMessage->ClearOffset();
		overviewMessage->Draw(info.overview.c_str());
	}

	lavelMessage->ClearOffset();
	for (int i = 0; i < info.level; i++) {
		lavelMessage->Draw("☆");
	}


	DrawSelectButton();
}