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

int JudgePlayerandFlyingObjectHit(int playerX, int playerY, int objectX, int objectY) {

	// “–‚½‚Á‚Ä‚¢‚éê‡‚Í‚P‚ğ•Ô‚µ‚Ü‚·
	if (playerX == objectX || playerY == objectY) return 1;

	// “–‚½‚Á‚Ä‚¢‚È‚¢ê‡‚Í‚O‚ğ•Ô‚µ‚Ü‚·
	return 0;

}

