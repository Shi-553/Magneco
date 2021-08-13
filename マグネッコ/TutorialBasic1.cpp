#include "TutorialBasic1.h"
#include "sprite.h"
#include "config.h"
#include "texture.h"
#include "InputLogger.h"
#include "time.h"
#include "player.h"
#include "map.h"
#include "npc.h"
#include "sceneManager.h"


#define POS_Y (120)

#define MESSAGEBOX_WIDTH (700)
#define MESSAGEBOX_HEIGHT (135)

#define MESSAGEBOX_X (SCREEN_WIDTH/2- MESSAGEBOX_WIDTH/2)
#define MESSAGEBOX_Y (440+POS_Y)

#define MESSAGE_X (MESSAGEBOX_X+30)
#define MESSAGE_Y (MESSAGEBOX_Y+20)

void TutorialBasic1::Init() {
	message = new Message();
	message->SetScale(D3DXVECTOR2(0.7, 0.7));
	message->SetMargin(7);
	message->SetPos(D3DXVECTOR2(MESSAGE_X, MESSAGE_Y));

	tutorialBasic1TextureIds[MESSAGE_BOX] = ReserveTextureLoadFile("texture/ui/Textbox_Test.png");
}
void TutorialBasic1::Uninit() {
	delete message;
}
void TutorialBasic1::Draw() {
	message->ClearOffset();

	switch (num)
	{
	case 0:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("チュートリアル基礎編その１へようこそ！");
		break;
	case 1:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("\"マグネッコ\"はルートを作って\n女の子をゴールまで誘導するゲームです！");
		break;
	case 2:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("基礎編その１ではゲームクリアまでの流れを\n実践してもらいます！");
		break;
	case 3:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("--ブロック取得＆ルート作成--(動画)");
		break;

	case 4:
	case 5:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("移動：左スティック\nルート作成：ルートのとなりで〇ボタン長押し");
		break;
	case 6:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("そのまま右下のゴールまで繋げましょう！");
		break;
	case 7:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("--誘導＆ゴール--(動画)");
		break;
	case 8:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("誘導：Xボタン");
		break;
	case 9:
		DrawSprite(tutorialBasic1TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("そのまま右下のゴールに誘導しましょう！");
		break;


	}
}
void TutorialBasic1::Update()  {
	switch (num)
	{
	case 0:
	case 1:
	case 2:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;
	case 3:
		isUpdate = false;

		//movie = new Movie();

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;
	case 4:
		isUpdate = true;


		if (GetPlayer()->putFlyingObjectList.size()!=0) {
			num++;
		}
		break;
	case 5:
		isUpdate = true;

		if (GetPlayer()->putFrame >= GetPlayer()->putRequiredFrame-1) {
			num++;
		}
		break;
	case 6:
		isUpdate = true;

		if((GetMapType(INTVECTOR2(GetMapWidth() - 3, GetMapHeight() - 2))==MAP_BLOCK) ||
		(GetMapType(INTVECTOR2(GetMapWidth() - 2, GetMapHeight() - 3))==MAP_BLOCK)) {
			num++;
		}
		break;
	case 7:
		isUpdate = false;

		//movie = new Movie();
		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;
	case 8:
		isUpdate = true;

		if (GetNpc()->isMove) {
			num++;
		}
		break;

	}
}