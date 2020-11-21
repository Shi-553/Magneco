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
bool CheckBlockBlock(D3DXVECTOR2& pos1, D3DXVECTOR2& pos2);


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
bool CheckBlockBlock(D3DXVECTOR2& pos1, D3DXVECTOR2& pos2) {
	return (
		(
			(pos1.x - 0.5f <= pos2.x - 0.5f && pos2.x - 0.5f <= pos1.x + 0.5f)||
			(pos1.x - 0.5f <= pos2.x + 0.5f && pos2.x + 0.5f <= pos1.x + 0.5f)
		)&&
		(
			(pos1.y - 0.5f <= pos2.y - 0.5f && pos2.y - 0.5f <= pos1.y + 0.5f)||
			(pos1.y - 0.5f <= pos2.y + 0.5f && pos2.y + 0.5f <= pos1.y + 0.5f)
		)
		);
}
bool CheckCollision(std::list<FlyingObject>* flyingObjectList, INTVECTOR2* pos) {
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); itr++) {
		if (itr->trans.GetIntPos()== *pos) {
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
			if (CheckBlockBlock(player->trans.pos, itr->trans.pos)) {
				auto move = (itr->trans.GetIntLastPos() - itr->trans.GetIntPos()).ToD3DXVECTOR2();

				itr->trans.pos = player->trans.pos;
				itr->trans.UpdatePos();

				bool isMove = player->dir != INTVECTOR2(0, 0);
				auto movePlayer = (player->dir).ToD3DXVECTOR2();

				while (true) {
					auto intPos = itr->trans.GetIntPos();
					if (player->trans.GetIntPos() != itr->trans.GetIntPos() && !CheckCollision(&player->flyingObjectList, &intPos)) {
						break;
					}

					if (isMove) {
						itr->trans.pos += movePlayer;
						//itr->trans.pos += player->dir.ToD3DXVECTOR2();
					}
					else {
						itr->trans.pos += move;
					}
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
			if (CheckBlockBlock(player->trans.pos, itr->trans.pos)) {
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
		for (auto itr2 = player->flyingObjectList.begin(); itr2 != player->flyingObjectList.end(); itr2++) {
			if (itr->type == FLYING_OBJECT_BLOCK) {
				if (player->flyingObjectList.size() >= MAX_BLOCK) {
					break;
				}
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos)) {
					auto move = (itr->trans.GetIntLastPos() - itr->trans.GetIntPos()).ToD3DXVECTOR2();

					itr->trans.pos = player->trans.pos;
					itr->trans.UpdatePos();

					bool isMove = player->dir != INTVECTOR2(0, 0);
					auto movePlayer = player->dir.ToD3DXVECTOR2();

					while (true) {
						auto intPos = itr->trans.GetIntPos();
						if (player->trans.GetIntPos() != itr->trans.GetIntPos() && !CheckCollision(&player->flyingObjectList, &intPos)) {
							break;
						}
						if (isMove) {
							itr->trans.pos += movePlayer;
						}
						else {
							itr->trans.pos += move;
						}
						itr->trans.UpdatePos();
					}

					player->flyingObjectList.push_back(*itr);
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}
			}
			else if (itr->type == FLYING_OBJECT_ENEMY) {
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos)) {
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

