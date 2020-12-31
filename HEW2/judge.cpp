#include "player.h"
#include "npc.h"
#include "flyingObject.h"
#include "judge.h"
#include "game.h"
#include "sceneManager.h"
#include "trans.h"
#include "map.h"
#include "npc.h"
#include "debugPrintf.h"



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
			(pos1.x - colllisionSize <= pos2.x - colllisionSize && pos2.x - colllisionSize <= pos1.x + colllisionSize) ||
			(pos1.x - colllisionSize <= pos2.x + colllisionSize && pos2.x + colllisionSize <= pos1.x + colllisionSize)
			) &&
		(
			(pos1.y - colllisionSize <= pos2.y - colllisionSize && pos2.y - colllisionSize <= pos1.y + colllisionSize) ||
			(pos1.y - colllisionSize <= pos2.y + colllisionSize && pos2.y + colllisionSize <= pos1.y + colllisionSize)
			)
		);
}
bool CheckCollision(std::list<FlyingObject>* flyingObjectList, INTVECTOR2* pos) {
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); itr++) {
		if (itr->trans.GetIntPos() == *pos) {
			return true;
		}
	}
	return false;
}

void JudgePlayerandFlyingObjectHit() {

	Player* player = GetPlayer();
	std::list<FlyingObject>* flyingObjectList = GetFlyingObjects();

	NPC* npc = GetNpc();


	// プレイヤーとflyingObjectの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if (!CheckBlockBlock(player->trans.pos, itr->trans.pos)) {
			itr++;
			continue;
		}
		if (itr->type == FLYING_OBJECT_BLOCK || itr->type == FLYING_OBJECT_CHECKPOINT_OFF ) {
			if (player->flyingObjectList.size() >= player->blockMax || player->checkCheckpoint) {
				itr++;
				continue;
			}
			 
			if (player->flyingObjectList.size() > 0 && itr->type == FLYING_OBJECT_CHECKPOINT_OFF) {
				itr++;
				continue;
			}

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



			if (itr->type == FLYING_OBJECT_CHECKPOINT_OFF) {

				player->checkCheckpoint = true;
			}
			else
			{
				itr->type = FLYING_OBJECT_PLAYER_BLOCK;
			}
			

			player->flyingObjectList.push_back(*itr);
			itr = flyingObjectList->erase(itr);

		}
		// 敵(ufo&enemy)とプレイヤー
		else if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_UFO || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK) {
			itr = flyingObjectList->erase(itr);
			player->flyingObjectList.clear();
			player->checkCheckpoint = false;

			//GoNextScene(GameOverScene, FADE_IN);
			//return;
		}
		else if (IsFlyingObjectItem(itr->type)) {
			if (itr->type == FLYING_OBJECT_ITEM_ADD_SPEED) {
				player->speed++;
			}
			else if (itr->type == FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE) {
				player->blockMax++;
			}
			itr = flyingObjectList->erase(itr);
			continue;
		}
		else {
			itr++;
		}

	}

	// 引っ付いてるFlyingObjectとenemyの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		bool isMatched = false;
		for (auto itr2 = player->flyingObjectList.begin(); itr2 != player->flyingObjectList.end(); itr2++) {
			if (itr->type == FLYING_OBJECT_BLOCK) {
				if (player->flyingObjectList.size() >= player->blockMax || player->checkCheckpoint) {
					break;
				}
				if (player->flyingObjectList.size() > 0 && itr->type == FLYING_OBJECT_CHECKPOINT_OFF) {
					break;
				}
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos) ) {
					auto move = (itr->trans.GetIntLastPos() - itr->trans.GetIntPos()).ToD3DXVECTOR2();

					//itr->trans.pos = player->trans.pos;
					itr->trans.pos = itr2->trans.pos;

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
					}
					itr->type = FLYING_OBJECT_PLAYER_BLOCK;

					player->flyingObjectList.push_back(*itr);
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}
			}
			else if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK) {
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos)) {
					player->flyingObjectList.erase(itr2);
					itr = flyingObjectList->erase(itr);
					player->checkCheckpoint = false;
					isMatched = true;
					break;
				}
			}
			else if (itr->type == FLYING_OBJECT_UFO) {
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos)) {
					player->flyingObjectList.erase(itr2);
					itr = flyingObjectList->erase(itr);
					npc->takeOutFrame = 0;
					player->checkCheckpoint = false;
					isMatched = true;
					DestroyUFO();
					break;
				}
			}

		}


		if (!isMatched) {
			itr++;
		}

	}

	// パージされたFlyingObjectとenemyの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		bool isMatched = false;
		for (auto itr2 = player->purgeFlyingObjectList.begin(); itr2 != player->purgeFlyingObjectList.end(); itr2++) {
			if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK) {
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos)) {
					player->purgeFlyingObjectList.erase(itr2);
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}
			}
			else if (itr->type == FLYING_OBJECT_UFO) {
				if (CheckBlockBlock(itr->trans.pos, itr2->trans.pos)) {
					player->purgeFlyingObjectList.erase(itr2);
					itr = flyingObjectList->erase(itr);
					npc->takeOutFrame = 0;
					isMatched = true;
					DestroyUFO();
					break;
				}
			}

		}


		if (!isMatched) {
			itr++;
		}

	}


	//NPCとの当たり判定

	if (GetMapType(npc->trans.GetIntPos()) == MAP_GOAL) {
		GoNextScene(GameClearScene);
	}

	if (GetMapType(npc->trans.GetIntPos()) == MAP_CHEST_CLOSED) {
		OpenChest(npc->trans.GetIntPos());
	}
	
	auto checkPointOffMap = GetMap(npc->trans.GetIntPos());

	if (checkPointOffMap != NULL && checkPointOffMap->type == MAP_CHAECKPOINT_OFF) {
		checkPointOffMap->type = MAP_CHAECKPOINT_ON;
	}

	// npcとufo
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if (itr->type == FLYING_OBJECT_UFO) {
			D3DXVECTOR2 shiftPos = itr->trans.pos - ADD_UFO_POS;
			if (CheckBlockBlock(npc->trans.pos, shiftPos)) {
				npc->takeOutFrame++;
				if (npc->takeOutFrame >= TAKE_OUT_FRAME_LIMIT) {
					itr = flyingObjectList->erase(itr);
					GoNextScene(GameOverScene, FADE_IN);
					return;
				}

			}
		}
		itr++;
	}




	// enemyFlyingObjectと設置ブロックの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end();) {
		bool isMatched = false;
		if (itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK) {
			Map* map = GetMap(itr->trans.GetIntPos());
			if (map != NULL && map->type == MAP_BLOCK) {
				itr = flyingObjectList->erase(itr);
				map->type = MAP_BLOCK_NONE;
				isMatched = true;
				continue;
			}
		}
		itr++;
	}
}

