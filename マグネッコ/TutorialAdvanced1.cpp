#include "TutorialAdvanced1.h"
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

void TutorialAdvanced1::Init() {
	message = new Message();
	message->SetScale(D3DXVECTOR2(0.7, 0.7));
	message->SetMargin(7);
	message->SetPos(D3DXVECTOR2(MESSAGE_X, MESSAGE_Y));

	textureIds[MESSAGE_BOX] = ReserveTextureLoadFile("texture/ui/Textbox_Test.png");

	num = 0;
}
void TutorialAdvanced1::Uninit() {
	SafeDelete( message);
	if (movie != nullptr) {
		movie->Stop();
		movie->Uninit();
		SafeDelete(movie);
	}

}
void TutorialAdvanced1::Draw() {
	message->ClearOffset();

	switch (num)
	{
	case 0:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("チュートリアル応用編へようこそ！");
		break;
	case 1:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("ここからはゲームの基礎的なルールではなく\nゲーム中に登場する少し特殊なギミックについての\n説明になります");
		break;
	case 2:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それでは初めて行きましょう！");
		break;
	case 3:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("アイテムはステージ内に設置された緑の箱に\n格納されています");
		break;
	case 4:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("箱のある地点にローズを誘導し、取り出されたら中身を\nマグネッコが取得することで効果を発揮します！");
		break;
	case 5:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("見つけたらどんどんとって楽にステージを\n攻略していきましょう！");
		break;
	case 6:
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 0.5f));
		DrawSprite(textureIds[BACKGROUND], { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT });
		SetSpriteColor(D3DXCOLOR(1, 1, 1, 1));

		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });
		message->Draw("--アイテム取得--");
		break;
	case 7:
		DrawSprite(textureIds[MESSAGE_BOX], { MESSAGEBOX_X ,  MESSAGEBOX_Y }, 10, { MESSAGEBOX_WIDTH, MESSAGEBOX_HEIGHT });

		message->Draw("それではアイテムを活用してゴールを目指しましょう");
		break;
	}
}
void TutorialAdvanced1::Update() {
	switch (num)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
		}
		break;
	case 5:
		isUpdate = false;


		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			movie = new Movie(L"movie/item.avi");
			auto& size = movie->GetSrcRect();
			movie->Init();
			movie->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2 - size.right / 2, SCREEN_HEIGHT / 2 - size.bottom / 2 - 50));
			movie->Play();
			movie->SetIsLoop(true);
		}
		break;
	case 6:
		isUpdate = false;

		if (TriggerInputLogger(MYVK_ENTER)) {
			num++;
			auto& size = movie->GetSrcRect();
			movie->SetPos(D3DXVECTOR2(100, 300));
			movie->SetSize(D3DXVECTOR2(size.right / 2, size.bottom / 2));
		}
		break;
	case 7:
		isUpdate = true;
	}
}