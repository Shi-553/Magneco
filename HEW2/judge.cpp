// --------------------------------------------------------------
//  “–‚½‚è”»’è‚Ìˆ—[judge.cpp]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include "player.h"
#include "flyingObject.h"
#include "judge.h"

void InitJudge(){

}

void UninitJudge() {
	//test
}

void UpdateJudge(){
	JudgePlayerandFlyingObjectHit();
}

void DrawJudge(){

}

void JudgePlayerandFlyingObjectHit() {


	Player* player = GetPlayer();
	std::list<FlyingObject>* flyingObjectList = GetFlyingObjects();

	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if ((int)player->position.x == (int)itr->pos.x&& (int)player->position.y == (int)itr->pos.y){
			player->flyingObjectList.push_back(*itr);
			itr->pos = itr->lastPos;
			itr = flyingObjectList->erase(itr);
		}else {
			itr++;
		}
	}




}

