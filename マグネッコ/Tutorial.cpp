#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "debugFont.h"
#include "player.h"
#include "npc.h"
#include "effect.h"
#include "gameSrite.h"
#include "game.h"

#include "Input.h"
#include "judge.h"

#include "map.h"
#include "flyingObjectSponer.h"
#include "debugPrintf.h"
#include "importExport.h"
#include "stageInfo.h"
#include "sound.h"

#include "Tutorial.h"
#include <map>

#include <memory>


#include "TutorialBasic1.h"
#include "TutorialBasic2.h"
#include "TutorialAdvanced1.h"



std::map<int, std::shared_ptr<ITutorial>> tutorialMap;
StageInfo* info;
std::shared_ptr<ITutorial> current = nullptr;

void RegisterTutorial(const std::shared_ptr<ITutorial>& tutorial);


void InitTutorial()
{
	RegisterTutorial(std::make_shared<TutorialBasic1>());
	RegisterTutorial(std::make_shared<TutorialBasic2>());
	RegisterTutorial(std::make_shared<TutorialAdvanced1>());


	InitStageInfo();

	InitPlayer();
	InitInput();
	InitNPC();
	InitEffect();
	InitMap();
	InitJudge();
	InitFlyingObject();
	InitFlyingSponer();
	//Grid_Initialize(GAME_SPRITE_WHIDTH, GetMapWidth(), D3DCOLOR_RGBA(0, 197, 0, 255));

	//StageExport();
	info = StageImport();
	if (info != nullptr) {
		StopSound();
		PlaySound(info->sLabel);

		auto itr = tutorialMap.find(info->index);
		if (itr == tutorialMap.end()) {
			current = nullptr;
		}
		else {
			current = (itr->second);
			current->Init();
		}
	}
	LoadTexture();
}

void UpdateTutorial()
{


	if (current != nullptr) {
		current->Update();
		if (!current->isUpdate) {
			return;
		}

		UpdateMap();
		UpdatePlayer();

		UpdateInput();

		UpdateNPC();
		UpdateEffect();

		UpdateJudge();
		UpdateFlyingObject();
		UpdateFlyingSponer();
	}
}

void DrawTutorial()
{
	DrawMap();
	DrawNPC();


	DrawInput();

	DrawJudge();
	DrawFlyingObject();
	DrawFlyingSponer();

	DrawEffect();

	DrawPlayer();

	if (current != nullptr) {
		current->Draw();
	}
}

void UninitTutorial()
{
	if (current != nullptr) {
		current->Uninit();
	}
	UninitInput();
	UninitPlayer();
	UninitNPC();
	UninitEffect();
	UninitJudge();
	UninitMap();

	UninitFlyingObject();
	UninitFlyingSponer();

	UninitStageInfo();
}


void RegisterTutorial(const std::shared_ptr<ITutorial>& tutorial) {
	tutorialMap.emplace(tutorial->GetStageIndex(), tutorial);
}