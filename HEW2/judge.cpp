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

bool CheckCollision(std::list<FlyingObject>* flyingObjectList, FlyingObject &flyingObject);


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

template <typename P1, typename P2, typename S1, typename S2>
bool CheckBlockBlock(P1& pos1, P2& pos2, S1& size1, S2& size2) {

	auto left1   = pos1.x - size1.x / 2;
    auto top1    = pos1.y - size1.y / 2; // 1.5, 1.5 
	auto right1  = pos1.x + size1.x / 2;
	auto bottom1 = pos1.y + size1.y / 2; // 2.5, 2.5

	auto left2   = pos2.x - size2.x / 2;
	auto top2    = pos2.y - size2.y / 2; // 1.5, 1.5 
	auto right2  = pos2.x + size2.x / 2;
	auto bottom2 = pos2.y + size2.y / 2; // 2.5, 2.5

	if (right1 >= left2 && left1 <= right2) {
		if (bottom1 >= top2 && top1 <= bottom2) {
			return true;
		}
	}
}
bool CheckCollision(std::list<FlyingObject>* flyingObjectList, FlyingObject &flyingObject) {

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


	// プレイヤーとflyingObjectの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if (!CheckBlockBlock(player->trans.pos, itr->trans.pos, player->size, itr->size)) {
			itr++;
			continue;
		}
		if (itr->type == FLYING_OBJECT_BLOCK || itr->type == FLYING_OBJECT_CHECKPOINT_OFF) {
			if (player->flyingObjectList.size() >= player->blockMax || player->checkCheckpoint || player->isPut) {
				itr++;
				continue;
			}

			if (player->flyingObjectList.size() > 0 && itr->type == FLYING_OBJECT_CHECKPOINT_OFF) {
				itr++;
				continue;
			}

			itr->trans.pos = player->trans.pos;
			itr->trans.UpdatePos();


			auto move = (itr->trans.GetIntLastPos() - itr->trans.GetIntPos()).ToD3DXVECTOR2();
			if (player->dir != D3DXVECTOR2(0, 0)) {
				move = D3DXVECTOR2(0, 0);
				if (fabsf(player->dir.x) > fabsf(player->dir.y)) {
					move.x = player->dir.x > 0 ? 1 : -1;
				}
				else {
					move.y = player->dir.y > 0 ? 1 : -1;
				}
			}

			if (move.x == 0 && move.y == 0) {
				move = itr->dir;
			}

			if (move.x == 0 && move.y == 0) {
				move = (player->trans.GetIntLastPos() - player->trans.GetIntPos()).ToD3DXVECTOR2();
			}

			while (true) {
				auto intPos = itr->trans.GetIntPos();
				if (player->trans.GetIntPos() != itr->trans.GetIntPos() && !CheckCollision(&player->flyingObjectList, *itr)) {
					break;
				}

				itr->trans.pos += move;
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
		else if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_UFO || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK || itr->type == FLYING_OBJECT_BIG_ENEMY) {
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
			if (!CheckBlockBlock(itr->trans.pos, itr2->trans.pos, itr->size, itr2->size)) {
				continue;
			}

			if (itr->type == FLYING_OBJECT_BLOCK) {
				if (player->flyingObjectList.size() >= player->blockMax || player->checkCheckpoint || player->isPut) {
					break;
				}
				if (player->flyingObjectList.size() > 0 && itr->type == FLYING_OBJECT_CHECKPOINT_OFF) {
					break;
				}

				itr->trans.pos = itr2->trans.pos;
				itr->trans.UpdatePos();


				auto move = (itr->trans.GetIntLastPos() - itr->trans.GetIntPos()).ToD3DXVECTOR2();
				if (player->dir != D3DXVECTOR2(0, 0)) {
					move = D3DXVECTOR2(0, 0);
					if (fabsf(player->dir.x) > fabsf(player->dir.y)) {
						move.x = player->dir.x > 0 ? 1 : -1;
					}
					else {
						move.y = player->dir.y > 0 ? 1 : -1;
					}
				}

					if (move.x == 0 && move.y == 0) {
						move = itr->dir;
					}
					if (move.x == 0 && move.y == 0) {
						move = (player->trans.GetIntLastPos() - player->trans.GetIntPos()).ToD3DXVECTOR2();
					}
					while (true) {
						auto intPos = itr->trans.GetIntPos();
						if (player->trans.GetIntPos() != itr->trans.GetIntPos() && !CheckCollision(&player->flyingObjectList,*itr)) {
							break;
						}

					itr->trans.pos += move;
					itr->trans.UpdatePos();

				}


				itr->type = FLYING_OBJECT_PLAYER_BLOCK;

				player->flyingObjectList.push_back(*itr);
				itr = flyingObjectList->erase(itr);
				isMatched = true;
				break;

			}
			else if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK || itr->type == FLYING_OBJECT_UFO || itr->type == FLYING_OBJECT_BIG_ENEMY) {

				player->flyingObjectList.erase(itr2);
				player->checkCheckpoint = false;
				itr->hp--;
				if (itr->hp <= 0) {
					itr = flyingObjectList->erase(itr);
					isMatched = true;

					if (itr->type == FLYING_OBJECT_UFO) {
						npc->takeOutFrame = 0;
						DestroyUFO();
					}
				}
				break;

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
			if (!CheckBlockBlock(itr->trans.pos, itr2->trans.pos, itr->size, itr2->size)) {
				continue;
			}
			if (itr->type == FLYING_OBJECT_ENEMY || itr->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK || itr->type == FLYING_OBJECT_UFO) {
				player->purgeFlyingObjectList.erase(itr2);

				itr->hp--;
				if (itr->hp <= 0) {
					itr = flyingObjectList->erase(itr);
					isMatched = true;

					if (itr->type == FLYING_OBJECT_UFO) {
						npc->takeOutFrame = 0;
						DestroyUFO();

					}
				}
				break;

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

