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
#include "sound.h"
#include <vector>

using namespace std;


bool CheckShortest(Player& player, FlyingObject& obj, D3DXVECTOR2& pos);

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

	NPC* npc = GetNpc();

	while (true) {
		bool resetFlyingObjectList = false;
		// 引っ付いてるFlyingObjectとenemyの当たり判定
		for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
			bool isMatched = false;
			for (auto itr2 = player->flyingObjectList.begin(); itr2 != player->flyingObjectList.end(); ) {

				if (!CheckBlockBlock(itr->trans.pos, itr2->trans.pos, itr->size, itr2->size)) {
					itr2++;
					continue;
				}
				if (!CheckShortest(*player,*itr, itr2->trans.pos)) {
					itr2++;
					continue;
				}

				if (IsFlyingObjectBlock(itr->type)) {
					if (GetBlock(*itr, itr2->trans.pos)) {
						itr = flyingObjectList->erase(itr);
						isMatched = true;
						break;
					}
					itr2++;
					continue;

				}
				else if (IsFlyingObjectEnemy(itr->type)) {
					if (IsPlayerInvicible()) {
						itr2++;
						continue;
					}

					itr2 = player->flyingObjectList.erase(itr2);
					resetFlyingObjectList = RemoteBlockToFreeFlyingObject();

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
					if (resetFlyingObjectList) {
						break;
					}
					continue;

				}
				else {
					itr2++;
				}
			}

			if (resetFlyingObjectList) {
				break;
			}

			if (!isMatched) {
				itr++;
			}

		}
		if (!resetFlyingObjectList) {
			break;
		}
	}
	// プレイヤーとflyingObjectの当たり判定
	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if (!CheckBlockBlock(player->trans.pos, itr->trans.pos, player->size, itr->size)) {
			itr++;
			continue;
		}

		if (IsFlyingObjectItem(itr->type)) {
			if (itr->type == FLYING_OBJECT_ITEM_ADD_SPEED) {
				player->addSpeed++;
			}
			else if (itr->type == FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE) {
				player->blockMax++;
			}
			itr = flyingObjectList->erase(itr);
			continue;
		}

		if (!CheckShortest(*player, *itr, player->trans.pos)) {
			itr++;
			continue;
		}

		if (IsFlyingObjectBlock(itr->type)) {

			if (GetBlock(*itr, player->trans.pos)) {
				itr = flyingObjectList->erase(itr);
			}
			else {
				itr++;
			}

		}
		// 敵(ufo&enemy)とプレイヤー
		else if (IsFlyingObjectEnemy(itr->type)) {

			if (DamagePlayer()) {
				itr->hp--;
				if (itr->hp <= 0) {

					if (itr->type == FLYING_OBJECT_UFO) {
						npc->takeOutFrame = 0;
						DestroyUFO();
					}
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
					itr = flyingObjectList->erase(itr);
					continue;
				}
			}

			itr++;

			//GoNextScene(GameOverScene, FADE_IN);
			//return;
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
			if (IsFlyingObjectEnemy(itr->type)) {
				itr2 = player->purgeFlyingObjectList.erase(itr2);
				
				itr->hp--;
				if (itr->hp <= 0) {

					if (itr->type == FLYING_OBJECT_UFO) {
						npc->takeOutFrame = 0;
						DestroyUFO();

					}
					PlaySound(SOUND_LABEL_SE_EXPLOSION);
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
				StopSound(SOUND_LABEL_SE_UFO);
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
		if (IsFlyingObjectBreakBlockEnemy(itr->type )) {
			INTVECTOR2 pos = itr->trans.GetIntPos();
			Map* map = GetMap(pos);
			if (map != NULL && map->type == MAP_BLOCK) {
				itr = flyingObjectList->erase(itr);
				map->type = MAP_BLOCK_NONE;
				isMatched = true;

				//チェックポイントとつながってないブロックを消す
				BreakNotConnectBlock(pos + INTVECTOR2(0, 1));
				BreakNotConnectBlock(pos + INTVECTOR2(0, -1));
				BreakNotConnectBlock(pos + INTVECTOR2(1,0));
				BreakNotConnectBlock(pos + INTVECTOR2(-1,0));
				continue;
			}
		}
		itr++;
	}
}


bool CheckShortest(Player& player,FlyingObject& obj, D3DXVECTOR2& pos) {
	auto posToObj = pos - obj.trans.pos;
	float len = D3DXVec2LengthSq(&posToObj);

	auto playerToObj = player.trans.pos - obj.trans.pos;
	if (len > D3DXVec2LengthSq(&playerToObj)) {
		return false;
	}

	for (auto itr = player.flyingObjectList.begin(); itr != player.flyingObjectList.end(); itr++) {
		auto posToFlyingObject = itr->trans.pos - obj.trans.pos;

		if (len > D3DXVec2LengthSq(&posToFlyingObject)) {
			return false;
		}
	}
	return true;
}