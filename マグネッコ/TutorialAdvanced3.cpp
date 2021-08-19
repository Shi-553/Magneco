#include "TutorialAdvanced3.h"
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

void TutorialAdvanced3::Init() {
	TextureIds[BACKGROUND] = ReserveTextureLoadFile("texture/fade.png");

	message = new Message();
	message->SetScale(D3DXVECTOR2(0.7, 0.7));
	message->SetMargin(7);
	message->SetPos(D3DXVECTOR2(MESSAGE_X, MESSAGE_Y));
	num = 0;

	TextureIds[MESSAGE_BOX] = ReserveTextureLoadFile("texture/ui/Textbox_Test.png");

}
void TutorialAdvanced3::Uninit() {
	delete message;

	movie->Stop();
	movie->Uninit();
	delete movie;

	ReleaseTexture(TextureIds, MAX);

}
void TutorialAdvanced3::Draw() {
	message->ClearOffset();

	switch (num)
	{
	case 0:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("今回は知っておくとステージ攻略が\n楽になる未起動ブロックについて説明します");
		break;
	case 1:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それでは始めていきましょう！");
		break;
	case 2:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("未起動ブロックはローズを誘導することで\n起動することが出来ます");
		break;
	case 3:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("見かけたらすぐルートを作ってローズを誘導する\nように心掛けましょう！");
		break;

	case 4:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(TextureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("--ブロック起動--(動画)");
		break;

	case 5:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それではブロックを起動してゴールを目指してみましょう");
		break;

	case 6:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("誘導：Xボタン");
		break;


	}
}
void TutorialAdvanced3::Update() {
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
		isUpdate = true;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;

	case 3:
		isUpdate = true;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie = new Movie(L"movie/checkpoint.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;

	case 4:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			auto& size = movie->GetSrcRect();
			movie->SetPos(D3DXVECTOR2(100, 300));
			movie->SetSize(D3DXVECTOR2(size.right / 2, size.bottom / 2));
		}
		break;

	case 5:
		isUpdate = true;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;



	}
}