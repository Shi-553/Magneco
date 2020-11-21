// --------------------------------------------------------------
//  “–‚½‚è”»’è‚Ìˆ—[judge.cpp]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include "player.h"
#include "flyingObject.h"
#include "judge.h"
#include "game.h"
#include "sceneManager.h"
#include "trans.h"

#define MAX_BLOCK (4)

bool CheckCollision(std::list<FlyingObject>* flyingObjectList, INTVECTOR2* pos);


void InitJudge() {

}

void UninitJudge() {
	//test
}

void UpdateJudge() {

	JudgePlayerandFlyingObjectHit();
}

void DrawJudge() {

}

bool CheckCollision(std::list<FlyingObject>* flyingObjectList,INTVECTOR2* pos ) {
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); itr++) {
		if (itr->trans.GetIntPos()==*pos) {
			return true;
		}
	}
	return false;
}

void JudgePlayerandFlyingObjectHit() {

	Player* player = GetPlayer();
	std::list<FlyingObject>* flyingObjectList = GetFlyingObjects();


	// プレイヤーとflyingObjectの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if (itr->type == FLYING_OBJECT_BLOCK) {
			if (player->flyingObjectList.size() >= MAX_BLOCK) {
				itr++;
				continue;
			}
			if (player->trans.GetIntPos()==itr->trans.GetIntPos()) {
				if (!player->isMove) {
					itr->trans.pos = itr->trans.GetIntLastPos().ToD3DXVECTOR2();
					itr->trans.UpdatePos();
				}
				auto intPos = itr->trans.GetIntPos();
				while (player->trans.GetIntPos() == itr->trans.GetIntPos() || CheckCollision(&player->flyingObjectList, &intPos)) {
					itr->trans.pos +=	(player->trans.GetIntPos() - player->trans.GetIntLastPos()).ToD3DXVECTOR2();
					itr->trans.UpdatePos();
				}

				itr->trans.Init(itr->trans.pos);

				player->flyingObjectList.push_back(*itr);
				itr = flyingObjectList->erase(itr);
			}
			else {
				itr++;
			}

		}
		else if (itr->type == FLYING_OBJECT_ENEMY) {
			if (player->trans.GetIntPos() == itr->trans.GetIntPos()) {
				itr = flyingObjectList->erase(itr);
				GoNextScene(GameOverScene, FADE_IN);
				return;
			}
			else {
				itr++;
			}

		}
	}

	// flyingObject同士の当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		bool isMatched = false;
		if (itr->type == FLYING_OBJECT_BLOCK) {
			if (player->flyingObjectList.size() >= MAX_BLOCK) {
				// playerにくっついているblockの数が４個以上
				itr++;
				continue;
			}
			// playerにくっついているblockの数が４個未満
			for (auto itr2 = player->flyingObjectList.begin(); itr2 != player->flyingObjectList.end(); itr2++) {
				if (itr->trans.GetIntPos() == itr2->trans.GetIntPos()) {
					if (!player->isMove) {
						itr->trans.pos = itr->trans.GetIntLastPos().ToD3DXVECTOR2();
						itr->trans.UpdatePos();
					}
					auto intPos = itr->trans.GetIntPos();
					while (player->trans.GetIntPos() == itr->trans.GetIntPos() || CheckCollision(&player->flyingObjectList, &intPos)) {
							itr->trans.pos += (player->trans.GetIntPos() - player->trans.GetIntLastPos()).ToD3DXVECTOR2();
							itr->trans.UpdatePos();
					}
					itr->trans.Init(itr->trans.pos);

					player->flyingObjectList.push_back(*itr);
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}
			}

		}
		else if (itr->type == FLYING_OBJECT_ENEMY) {
			// playerにくっついているflyingObjectにEnemyがぶつかった場合
			for (auto itr2 = player->flyingObjectList.begin(); itr2 != player->flyingObjectList.end(); itr2++) {
				if (itr->trans.GetIntPos() == itr2->trans.GetIntPos()) {
					player->flyingObjectList.erase(itr2);
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}

			}
		}
		if (!isMatched) {
			itr++;
		}

	}




}

