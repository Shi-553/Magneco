// --------------------------------------------------------------
//  “–‚½‚è”»’è‚Ìˆ—[judge.cpp]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include "player.h"
#include "flyingObject.h"
#include "judge.h"

#define MAX_BLOCK (4)


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
			if ((int)player->position.x == (int)itr->pos.x && (int)player->position.y == (int)itr->pos.y) {
				itr->pos = itr->lastPos;
				player->flyingObjectList.push_back(*itr);
				itr = flyingObjectList->erase(itr);
			}
			else {
				itr++;
			}

		}
		else if (itr->type == FLYING_OBJECT_ENEMY) {
			if ((int)player->position.x == (int)itr->pos.x && (int)player->position.y == (int)itr->pos.y) {
				itr = flyingObjectList->erase(itr);
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
				if ((int)itr->pos.x == (int)itr2->pos.x && (int)itr->pos.y == (int)itr2->pos.y) {
					itr->pos = itr->lastPos;
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
				if ((int)itr->pos.x == (int)itr2->pos.x && (int)itr->pos.y == (int)itr2->pos.y) {
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

