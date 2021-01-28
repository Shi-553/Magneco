#include "staffRole.h"
#include "messeage.h"
#include "config.h"
#include <string>
#include "sceneManager.h"

static Message* staffRoleMessage;
static float frame = 0;
static std::string staffRole = 
R"(
スタッフロール

AT12G
名前
名前
名前
名前
名前
AT12G
名前
名前
名前
名前
名前
AT12G
名前
名前
名前
名前
名前


デバッガー
齊藤龍一

AT12G
名前
名前
名前
名前
名前
AT12G
名前
名前
名前
名前
名前
)";
static int staffRoleKaigyoCount = std::count(staffRole.cbegin(), staffRole.cend(), '\n')+2;
static int oneSizeHeight;
void InitStaffRole() {
	staffRoleMessage = new Message();
	frame = 0;
	oneSizeHeight = staffRoleMessage->GetOneSize().y;
}
void UninitStaffRole() {
	delete staffRoleMessage;
}
void UpdateStaffRole() {
	frame += 1;
	if (-SCREEN_HEIGHT +frame > staffRoleKaigyoCount * oneSizeHeight) {
		GoNextScene(GameStartScene);
	}
}
void DrawStaffRole() {
	staffRoleMessage->ClearOffset();
	staffRoleMessage->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT - frame));
	staffRoleMessage->SetEndPos(D3DXVECTOR2(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT - frame));
	staffRoleMessage->SetFormat(DT_CENTER | DT_NOCLIP);
	staffRoleMessage->Draw(staffRole.c_str());
}