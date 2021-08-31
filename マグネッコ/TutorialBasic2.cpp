#include "TutorialBasic2.h"
#include "sprite.h"
#include "config.h"
#include "texture.h"
#include "InputLogger.h"
#include "time.h"
#include "player.h"
#include "map.h"
#include "npc.h"
#include "sceneManager.h"
#include "SafeDelete.hpp"


#define POS_Y (120)

#define MESSAGEBOX_WIDTH (700)
#define MESSAGEBOX_HEIGHT (135)

#define MESSAGEBOX_X (SCREEN_WIDTH/2- MESSAGEBOX_WIDTH/2)
#define MESSAGEBOX_Y (440+POS_Y)

#define MESSAGE_X (MESSAGEBOX_X+30)
#define MESSAGE_Y (MESSAGEBOX_Y+20)

void TutorialBasic2::Init() {

	textureIds[BACKGROUND] = ReserveTextureLoadFile("texture/fade.png");

	message = new Message();
	message->SetScale(D3DXVECTOR2(0.7, 0.7));
	message->SetMargin(7);
	message->SetPos(D3DXVECTOR2(MESSAGE_X, MESSAGE_Y));

	textureIds[MESSAGE_BOX] = ReserveTextureLoadFile("texture/ui/Textbox_Test.png");

	num = 0;
}
void TutorialBasic2::Uninit() {
	SafeDelete(message);

	if (movie != nullptr) {
		movie->Stop();
		movie->Uninit();
		SafeDelete(movie);
	}
}
void TutorialBasic2::Draw() {
	message->ClearOffset();

	switch (num)
	{
	case 0:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("チュートリアル基礎編その２へようこそ！");
		break;
	case 1:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("ここではゲームオーバーになってしまう条件や\nそれを防ぐ方法をお伝えします");
		break;
	case 2:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それでは初めて行きましょう！");
		break;
	case 3:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(textureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));


		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });
		message->Draw("今エリアを通っているのがエネミーです");

		break;

	case 4:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("マグネッコがぶつかるとブロックを手放して\n気絶してしまいます");
		break;
	case 5:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(textureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		break;
	case 6:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("エネミーに当たりそうでブロックを持っているなら\n射出を使いエネミーを倒してしまいましょう！");
		break;
	case 7:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(textureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });
		message->Draw("射出：R1ボタン");

		break;
	case 8:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("続いてはローズをさらおうとするUFOについてです");
		break;
	case 9:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("ローズに取りついてから一定時間が経過すると\nゲームオーバーになってしまいます");
		break;
	case 10:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("エネミーよりも耐久が高いため\n素早く射出を使い対処しましょう");
		break;
	case 11:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(textureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		break;
	case 12:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それでは実践です！\nエネミーとUFOに対処しながらルートを作り\nローズをゴールに導いてみましょう！");
		break;
	case 13:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("射出：R1ボタン");
		break;
	}
}
void TutorialBasic2::Update() {
	switch (num)
	{
	case 0:
	case 1:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}

		break;
	case 2:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie = new Movie(L"movie/B_EnemyThrough.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;
	case 3:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie->Uninit();
		}
		
		break;
	case 4:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie = new Movie(L"movie/B_EnemyCollide.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;
	case 5:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie->Uninit();
		}
		break;
	case 6:
		isUpdate = false;
		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie = new Movie(L"movie/B_EnemyKnockdown.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;
	case 7:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie->Uninit();
		}
		break;
	case 8:
	case 9:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;
	case 10:
		isUpdate = false;
		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie = new Movie(L"movie/B_UFO.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;
	case 11:
	case 12:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			auto& size = movie->GetSrcRect();
			movie->SetPos(D3DXVECTOR2(100, 300));
			movie->SetSize(D3DXVECTOR2(size.right / 2, size.bottom / 2));
		}
		break;
	case 13:
		isUpdate = true;
		break;
	}
}