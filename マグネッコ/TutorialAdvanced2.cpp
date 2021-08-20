#include "TutorialAdvanced2.h"
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

void TutorialAdvanced2::Init() {
	TextureIds[BACKGROUND] = ReserveTextureLoadFile("texture/fade.png");

	message = new Message();
	message->SetScale(D3DXVECTOR2(0.7, 0.7));
	message->SetMargin(7);
	message->SetPos(D3DXVECTOR2(MESSAGE_X, MESSAGE_Y));
	num = 0;

	TextureIds[MESSAGE_BOX] = ReserveTextureLoadFile("texture/ui/Textbox_Test.png");

}
void TutorialAdvanced2::Uninit() {
	delete message;

	movie->Stop();
	movie->Uninit();
	delete movie;

	ReleaseTexture(TextureIds, MAX);

}
void TutorialAdvanced2::Draw() {
	message->ClearOffset();

	switch (num)
	{
	case 0:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("今回の応用編では妨害ブロックと隕石について\n説明していきます！");
		break;
	case 1:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("どちらもマグネッコのルート作りを阻害する\nお邪魔な要素です");
		break;
	case 2:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それでは始めていきましょう！");
		break;
	case 3:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("ステージ上の穴が開いたブロックは妨害ブロックです");
		break;

	case 4:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("妨害ブロックのある場所には集めたブロックを\n置くことができません");
		break;

	case 5:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(TextureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("--妨害ブロックに重なってブロックを置くことができない--");
		break;

	case 6:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("また作ったルートを破壊してしまう\n隕石も存在します");
		break;
	case 7:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("当たった箇所だけでなく左上の磁石ブロックから\n繋がっていないルートはすべて崩れてしまいます");
		break;
	case 8:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("なるべく幅の太いルート作りを心掛け\n隕石が出現したら射出で早めに対処しましょう");
		break;
	case 9:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(TextureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("--隕石にルート壊される--");
		break;
	case 10:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("かなり邪魔になる隕石ですがメリットもあります");
		break;
	case 11:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("妨害ブロックに重なったタイミングで\n射出ブロックをぶつけると...");
		break;
	case 12:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(TextureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("--隕石で妨害ブロックを壊す--");
		break;
	case 13:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("このように妨害ブロックの破壊にも使えます");
		break;
	case 14:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それでは実践してみましょう！");
		break;
	case 15:
		DrawSprite(TextureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("射出：R1ボタン");
		break;




	}
}
void TutorialAdvanced2::Update() {
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

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;

	case 4:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie = new Movie(L"movie/bougai.avi");
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
			auto& size = movie->GetSrcRect();
			movie->SetPos(D3DXVECTOR2(100, 300));
			movie->SetSize(D3DXVECTOR2(size.right / 2, size.bottom / 2));
		}
		break;

	case 6:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;

	case 7:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			auto& size = movie->GetSrcRect();
			movie->SetPos(D3DXVECTOR2(100, 300));
			movie->SetSize(D3DXVECTOR2(size.right / 2, size.bottom / 2));
		}
		break;


	case 8:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie->Stop();
			movie->Uninit();
			delete movie;

			movie = new Movie(L"movie/break_block.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;

	case 9:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			auto& size = movie->GetSrcRect();
			movie->SetPos(D3DXVECTOR2(100, 300));
			movie->SetSize(D3DXVECTOR2(size.right / 2, size.bottom / 2));
		}
		break;

	case 10:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;

	case 11:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie->Stop();
			movie->Uninit();
			delete movie;

			movie = new Movie(L"movie/break_meteorite.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;

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
	case 14:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;

	case 15:
		isUpdate = true;


	}
}