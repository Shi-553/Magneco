#include "staffRole.h"
#include "messeage.h"
#include "config.h"
#include <string>
#include "sceneManager.h"
#include "texture.h"
#include "sprite.h"
#include "InputLogger.h"
#include "sound.h"
#include "SafeDelete.hpp"

#define STAFF_ROLE_TITLE_WIDTH 640
#define STAFF_ROLE_TITLE_HEIGHT 240

#define STAFF_ROLE_PRESS_O_WIDTH 128
#define STAFF_ROLE_PRESS_O_HEIGHT 32

static Message* staffRoleMessage;
static float frame = 0;
static std::string staffRole =
R"(
スタッフロール


チーム GamingFJ

リーダー・プログラマー

竹内　廉 


プランナー

緒方　進之助


プログラマ

柴田　晴彦

野田　栞那


デザイナー

西村 彩伽

伊藤 架

BONICALZI CECILIA



スペシャルサンクス

緒方 宏義

齊藤 龍一



アドバイザー

後藤 智成 先生




フォント素材

PixelMplus（ピクセル・エムプラス）



音源素材

OtoLogic https://otologic.jp

(C)PANICPUMPKIN http://pansound.com/panicpumpkin

On-Jin ～音人～ https://on-jin.com/ 




















Thank you for playing!!
)";
static int staffRoleKaigyoCount = std::count(staffRole.cbegin(), staffRole.cend(), '\n') + 2;
static int oneSizeHeight;
static int staffRoleId = TEXTURE_INVALID_ID;
static int staffRoleTitleId = TEXTURE_INVALID_ID;
static int pressOTexture = TEXTURE_INVALID_ID;
static int animeFrame = 0;

void InitStaffRole() {
	staffRoleMessage = new Message();
	frame = 0;
	animeFrame = 0;
	oneSizeHeight = staffRoleMessage->GetOneSize().y;
	staffRoleId = ReserveTextureLoadFile("texture/background/背景１.png");
	staffRoleTitleId = ReserveTextureLoadFile("texture/ui/title_640×240.png");
	pressOTexture = ReserveTextureLoadFile("texture/ui/press_o_128×32.png");
	PlaySound(SOUND_LABEL_BGM004);
}
void UninitStaffRole() {
	StopSound();
	SafeDelete(staffRoleMessage);
	ReleaseTexture(staffRoleId);
	ReleaseTexture(staffRoleTitleId);
	ReleaseTexture(pressOTexture);
}
void UpdateStaffRole() {
	if (-SCREEN_HEIGHT + frame > staffRoleKaigyoCount * oneSizeHeight - (SCREEN_HEIGHT / 2) - 200) {
		animeFrame++;
		if (TriggerInputLogger(MYVK_ENTER)) {
			GoNextScene(GameStartScene);
		}
	}
	else {
		frame += 2;
		if (PressInputLogger(MYVK_ENTER)) {
			frame += 3;
		}
	}
}
void DrawStaffRole() {
	SetSpriteColor(D3DCOLOR_RGBA(100, 100, 100, 255));
	DrawSprite(staffRoleId, D3DXVECTOR2(0, 0), 10, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	staffRoleMessage->ClearOffset();
	staffRoleMessage->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT - frame));
	staffRoleMessage->SetEndPos(D3DXVECTOR2(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT - frame));
	staffRoleMessage->SetFormat(DT_CENTER | DT_NOCLIP);
	staffRoleMessage->Draw(staffRole.c_str());


	DrawSprite(staffRoleTitleId, D3DXVECTOR2(SCREEN_WIDTH / 2 - STAFF_ROLE_TITLE_WIDTH / 2, (staffRoleKaigyoCount * oneSizeHeight) + 300 - frame), 10);
	if (animeFrame >= 96) {
		DrawSprite(pressOTexture, { 990,590 }, 10, D3DXVECTOR2(STAFF_ROLE_PRESS_O_WIDTH, STAFF_ROLE_PRESS_O_HEIGHT),
				D3DXVECTOR2(0, 32 * (((int)(animeFrame / 8.0)+7) % 8)), D3DXVECTOR2(STAFF_ROLE_PRESS_O_WIDTH, STAFF_ROLE_PRESS_O_HEIGHT));
	}
}