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
#include <vector>

using namespace std;



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


bool CheckCollision(std::list<FlyingObject>* flyingObjectList, FlyingObject& flyingObject) {

	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); itr++) {

		if (CheckBlockBlock(itr->trans.GetIntPos(), flyingObject.trans.GetIntPos(), itr->size, flyingObject.size)) {
			return true;
		}
	}
	return false;
}

void JudgePlayerandFlyingObjectHit() {

	Player* player = GetPlayer();
	std::list<FlyingObject>* flyingObjectList = GetFlyingObjects();

	NPC* npc = GetNpc();

	bool isDaletePlayerFlyingObject = false;
	// 引っ付いてるFlyingObjectとenemyの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		bool isMatched = false;
		for (auto itr2 = player->flyingObjectList.begin(); itr2 != player->flyingObjectList.end(); ) {

			if (!CheckBlockBlock(itr->trans.pos, itr2->trans.pos, itr->size, itr2->size)) {
				itr2++;
				continue;
			}

			if (itr->type == FLYING_OBJECT_BLOCK) {
				if (GetBlock(*itr, itr2->trans.pos)) {
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}
				itr2++;
				continue;

			}
			else if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK || itr->type == FLYING_OBJECT_UFO || itr->type == FLYING_OBJECT_ENEMY_SECOND) {
				if (IsPlayerInvicible()) {
					itr2++;
					continue;
				}

				itr2 = player->flyingObjectList.erase(itr2);
				isDaletePlayerFlyingObject = true;

				player->checkCheckpoint = false;
				itr->hp--;
				if (itr->hp <= 0) {

					if (itr->type == FLYING_OBJECT_UFO) {
						npc->takeOutFrame = 0;
						DestroyUFO();
					}
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}
				continue;

			}
			else {
				itr2++;
			}
		}


		if (!isMatched) {
			itr++;
		}

	}
	if (isDaletePlayerFlyingObject) {
		RemoteBlockToFreeFlyingObject();
	}

	// プレイヤーとflyingObjectの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {

		if (itr->type == FLYING_OBJECT_BLOCK || itr->type == FLYING_OBJECT_CHECKPOINT_OFF) {
			if (!CheckBlockBlock(player->trans.pos, itr->trans.pos, player->size, itr->size)) {
				itr++;
				continue;
			}

			if (GetBlock(*itr, player->trans.pos)) {
				itr = flyingObjectList->erase(itr);
			}
			else {
				itr++;
			}

		}
		// 敵(ufo&enemy)とプレイヤー
		else if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_UFO || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK || itr->type == FLYING_OBJECT_ENEMY_SECOND) {

			auto playerSize = player->size - D3DXVECTOR2(0.2, 0.2);
			if (!CheckBlockBlock(player->trans.pos, itr->trans.pos, playerSize, itr->size)) {
				itr++;
				continue;
			}
			if (DamagePlayer()) {
				itr->hp--;
				if (itr->hp <= 0) {

					if (itr->type == FLYING_OBJECT_UFO) {
						npc->takeOutFrame = 0;
						DestroyUFO();
					}
					itr = flyingObjectList->erase(itr);
					continue;
				}
			}

			itr++;

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
		}
		else {
			itr++;
		}

	}
	// パージされたFlyingObjectとenemyの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		bool isMatched = false;
		for (auto itr2 = player->purgeFlyingObjectList.begin(); itr2 != player->purgeFlyingObjectList.end();) {
			if (!CheckBlockBlock(itr->trans.pos, itr2->trans.pos, itr->size, itr2->size)) {
				itr2++;
				continue;
			}
			if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK || itr->type == FLYING_OBJECT_UFO || itr->type == FLYING_OBJECT_ENEMY_SECOND) {
				itr2 = player->purgeFlyingObjectList.erase(itr2);

				itr->hp--;
				if (itr->hp <= 0) {

					if (itr->type == FLYING_OBJECT_UFO) {
						npc->takeOutFrame = 0;
						DestroyUFO();

					}
					itr = flyingObjectList->erase(itr);
					isMatched = true;
					break;
				}
				continue;

			}
			else {
				itr2++;
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

	if (GetMapType(npc->trans.GetIntPos()) == MAP_CHAECKPOINT_ON) {
		SetNPCResponePos(npc->trans.GetIntPos());
	}

	// npcとufo
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if (itr->type == FLYING_OBJECT_UFO) {
			D3DXVECTOR2 shiftPos = itr->trans.pos - ADD_UFO_POS;
			if (CheckBlockBlock(npc->trans.pos, shiftPos, npc->size, itr->size)) {
				npc->takeOutFrame++;
				if (npc->takeOutFrame >= TAKE_OUT_FRAME_LIMIT) {
					//itr = flyingObjectList->erase(itr);
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
			INTVECTOR2 pos = itr->trans.GetIntPos();
			Map* map = GetMap(pos);
			if (map != NULL && map->type == MAP_BLOCK) {
				itr = flyingObjectList->erase(itr);
				map->type = MAP_BLOCK_NONE;
				isMatched = true;

				//チェックポイントとつながってないブロックを消す
				vector<INTVECTOR2> v;
				if (!IsBreakBlock(pos + INTVECTOR2(0, 1), v)) {
					for (auto itrV = v.begin(); itrV != v.end(); itrV++) {
						Map* map = GetMap(*itrV);
						if (map != NULL && map->type == MAP_BLOCK) {
							map->type = MAP_BLOCK_NONE;
						}
					}
				}
				v.clear();
				if (!IsBreakBlock(pos + INTVECTOR2(0, -1), v)) {
					for (auto itrV = v.begin(); itrV != v.end(); itrV++) {
						Map* map = GetMap(*itrV);
						if (map != NULL && map->type == MAP_BLOCK) {
							map->type = MAP_BLOCK_NONE;
						}
					}
				}
				v.clear();
				if (!IsBreakBlock(pos + INTVECTOR2(1, 0), v)) {
					for (auto itrV = v.begin(); itrV != v.end(); itrV++) {
						Map* map = GetMap(*itrV);
						if (map != NULL && map->type == MAP_BLOCK) {
							map->type = MAP_BLOCK_NONE;
						}
					}
				}
				v.clear();
				if (!IsBreakBlock(pos + INTVECTOR2(-1, 0), v)) {
					for (auto itrV = v.begin(); itrV != v.end(); itrV++) {
						Map* map = GetMap(*itrV);
						if (map != NULL && map->type == MAP_BLOCK) {
							map->type = MAP_BLOCK_NONE;
						}
					}
				}
				v.clear();
				continue;
			}
		}
		itr++;
	}
}
