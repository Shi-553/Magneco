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
static float colllisionSize = 0.3;

bool CheckBlockBlock(D3DXVECTOR2& pos1, D3DXVECTOR2& pos2) {
	return (
		(
			(pos1.x - colllisionSize <= pos2.x - colllisionSize && pos2.x - colllisionSize <= pos1.x + colllisionSize)||
			(pos1.x - colllisionSize <= pos2.x + colllisionSize && pos2.x + colllisionSize <= pos1.x + colllisionSize)
		)&&
		(
			(pos1.y - colllisionSize <= pos2.y - colllisionSize && pos2.y - colllisionSize <= pos1.y + colllisionSize)||
			(pos1.y - colllisionSize <= pos2.y + colllisionSize && pos2.y + colllisionSize <= pos1.y + colllisionSize)
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


				bool isMovePlayer = player->dir != D3DXVECTOR2(0, 0);
				D3DXVECTOR2 movePlayer = D3DXVECTOR2(0, 0);
				if (isMovePlayer) {
					if (fabsf(player->dir.x) > fabsf(player->dir.y)) {
						movePlayer.x = player->dir.x > 0 ? 1 : -1;
					}
					else {
						movePlayer.y = player->dir.y > 0 ? 1 : -1;
					}
				}

				while (true) {
					auto intPos = itr->trans.GetIntPos();
					if (player->trans.GetIntPos() != itr->trans.GetIntPos() && !CheckCollision(&player->flyingObjectList, &intPos)) {
						break;
					}

					if (isMovePlayer) {
						itr->trans.pos += movePlayer;
					}
					else {
						itr->trans.pos += move;
					}
					itr->trans.UpdatePos();
					intPos = itr->trans.GetIntPos();
				}

				itr->trans.Init(itr->trans.pos);

				itr->type = FLYING_OBJECT_PLAYER_BLOCK;
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

					//itr->trans.pos = player->trans.pos;
					itr->trans.pos = itr2->trans.pos;

					itr->trans.UpdatePos();

					bool isMovePlayer = player->dir != D3DXVECTOR2(0, 0);
					D3DXVECTOR2 movePlayer = D3DXVECTOR2(0, 0);
					if (isMovePlayer) {
						if (fabsf(player->dir.x)> fabsf(player->dir.y)) {
							movePlayer.x = player->dir.x > 0 ? 1 : -1;
						}
						else {
							movePlayer.y = player->dir.y > 0 ? 1 : -1;
						}
					}
					while (true) {
						auto intPos = itr->trans.GetIntPos();
						if (player->trans.GetIntPos() != itr->trans.GetIntPos() && !CheckCollision(&player->flyingObjectList, &intPos)) {
							break;
						}
						if (isMovePlayer) {
							itr->trans.pos += movePlayer;
						}
						else {
							itr->trans.pos += move;
						}
						itr->trans.UpdatePos();
					}
					itr->type = FLYING_OBJECT_PLAYER_BLOCK;

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

	// purgeFlyingObjectとenemyの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		bool isMatched = false;
		for (auto itr2 = player->purgeFlyingObjectList.begin(); itr2 != player->purgeFlyingObjectList.end(); itr2++) {
			if (itr->type == FLYING_OBJECT_ENEMY) {
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos)) {
					player->purgeFlyingObjectList.erase(itr2);
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

