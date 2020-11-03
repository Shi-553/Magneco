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

}

void DrawJudge(){

}

int JudgePlayerandFlyingObjectHit() {

	Player* player = GetPlayer();
	std::list<FlyingObject>* flyingObjectList = GetFlyingObjects();

	for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
		if (player->position == itr->pos){
			player->flyingObjectList.push_back(*itr);
			itr->pos = itr->lastPos;
			itr = flyingObjectList->erase(itr);
		}else {
			itr++;
		}
	}



}

