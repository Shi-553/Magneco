#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sceneManager.h"
#include "selectButton.h"
#include "sound.h"
#include "stageSelect.h"

#define GAME_CLEAR_BUTTON_WIDTH 330
#define GAME_CLEAR_BUTTON_HEIGHT 90

#define GAME_CLEAR_LOGO_WIDTH 1024
#define GAME_CLEAR_LOGO_HEIGHT 256

static int frame = 0;

static int backgroundTexture;
static int gameClearTexture;

static SelectButton gameClearSelect;

void InitGameClear()
{
	gameClearSelect.Init();
	StopSound();
	PlaySound(SOUND_LABEL_BGM003);


 Button returnTitleButton, stageSelectButton, nextButton,staffRoleButton;


	auto buttonCenter = D3DXVECTOR2(SCREEN_WIDTH / 2 - GAME_CLEAR_BUTTON_WIDTH / 2, (SCREEN_HEIGHT / 2 - GAME_CLEAR_BUTTON_HEIGHT / 2) + 30);
	bool canNext = IsNextStage();
	bool isMain = GetStageFoldername() == "stage/main";
	if (canNext) {
		nextButton.pos = buttonCenter ;
		stageSelectButton.pos = buttonCenter + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16);
		returnTitleButton.pos = stageSelectButton.pos + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16);
	}
	else if (isMain) {
		staffRoleButton.pos = buttonCenter + (D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16) / 3);
		returnTitleButton.pos = staffRoleButton.pos + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16);
	}
	else {
		stageSelectButton.pos = buttonCenter + (D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16)/3);
		returnTitleButton.pos = stageSelectButton.pos + D3DXVECTOR2(0, GAME_CLEAR_BUTTON_HEIGHT + 16);
	}

	if (canNext) {
		nextButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
		nextButton.textureId = ReserveTextureLoadFile("texture/ui/next.png");
		nextButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/next_pressed.png");


		nextButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
			NextStaege();
			GoNextScene(GameScene);
		};
		gameClearSelect.Add(nextButton);
	}

	if (!canNext&& isMain) {

		staffRoleButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
		staffRoleButton.textureId = ReserveTextureLoadFile("texture/ui/staffrole.png");
		staffRoleButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/staffrole_pressed.png");


		staffRoleButton.releasedCallback = []() {
			StopSound();
			PlaySound(SOUND_LABEL_SE_DECITION);
			GoNextScene(StaffRole);
		};
		gameClearSelect.Add(staffRoleButton);
	}
	else {

		stageSelectButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
		stageSelectButton.textureId = ReserveTextureLoadFile("texture/ui/stageselect.png");
		stageSelectButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/stageselect_pressed.png");


		stageSelectButton.releasedCallback = []() {
			StopSound();
			PlaySound(SOUND_LABEL_SE_DECITION);
			GoNextScene(StageSelect);
		};
		gameClearSelect.Add(stageSelectButton);
	}

	returnTitleButton.size = D3DXVECTOR2(GAME_CLEAR_BUTTON_WIDTH, GAME_CLEAR_BUTTON_HEIGHT);
	returnTitleButton.textureId = ReserveTextureLoadFile("texture/ui/quit.png");
	returnTitleButton.pressedTextureId = ReserveTextureLoadFile("texture/ui/quit_pressed.png");

	returnTitleButton.releasedCallback = []() {
		PlaySound(SOUND_LABEL_SE_DECITION);
		GoNextScene(GameStartScene);
	};
	gameClearSelect.Add(returnTitleButton);

	backgroundTexture = ReserveTextureLoadFile("texture/background/背景5.jpg");
	gameClearTexture = ReserveTextureLoadFile("texture/ui/stageclear_1024×256.png");

	gameClearSelect.SetFrame(ReserveTextureLoadFile("texture/ui/select.png"));

	LoadTexture();

	frame = 0;
}

void UninitGameClear()
{
	gameClearSelect.Uninit();

	ReleaseTexture(backgroundTexture);
	ReleaseTexture(gameClearTexture);
}

void DrawGameClear()
{
	DrawSprite(backgroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });

		auto tPos = D3DXVECTOR2(
			0,
			GAME_CLEAR_LOGO_HEIGHT * (frame / 16 % 4)
		);

	DrawSprite(gameClearTexture, { 200,64 }, 10, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT }, tPos, { GAME_CLEAR_LOGO_WIDTH,GAME_CLEAR_LOGO_HEIGHT });
	gameClearSelect.DrawMain();
	gameClearSelect.DrawLR();

}

void UpdateGameClear()
{
	gameClearSelect.Update();

	frame++;
}


