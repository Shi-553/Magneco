// --------------------------------------------------------------
//  “–‚½‚è”»’è‚Ìˆ—[judge.cpp]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include "player.h"
#include "flyingObject.h"
#include "judge.h"
#include "game.h"

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
bool CheckCollision(D3DXVECTOR2* pos1,D3DXVECTOR2* pos2 ) {
	return (int)pos1->x == (int)pos2->x && (int)pos1->y == (int)pos2->y;
}
bool CheckCollision(std::list<FlyingObject>* flyingObjectList,D3DXVECTOR2* pos ) {
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); itr++) {
		if (CheckCollision(pos,&itr->pos)) {
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
			if (CheckCollision(&player->position,&itr->pos)) {
				itr->pos = itr->lastPos;
				if (CheckCollision(&player->flyingObjectList, &itr->pos)) {
					itr->pos += player->position - player->lastPosition;
				}

				player->flyingObjectList.push_back(*itr);
				itr = flyingObjectList->erase(itr);
			}
			else {
				itr++;
			}

		}
		else if (itr->type == FLYING_OBJECT_ENEMY) {
			if (CheckCollision(&player->position, &itr->pos)) {
				itr = flyingObjectList->erase(itr);
				InitGame();
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
				if (CheckCollision(&itr->pos, &itr2->pos)) {
					itr->pos = itr->lastPos;
					if (CheckCollision(&player->flyingObjectList, &itr->pos)|| CheckCollision(&itr->pos, &player->position)) {
						itr->pos += player->position - player->lastPosition;
					}
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
				if (CheckCollision(&itr->pos, &itr2->pos)) {
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

