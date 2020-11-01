// --------------------------------------------------------------
//  “–‚½‚è”»’è‚Ìˆ—[judge.h]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#ifndef _JUDGE_H
#define _JUDGE_H

void InitJudge();
void UninitJudge();
void UpdateJudge();
void DrawJudge();

void JudgePlayerandFlyingObjectHit(int playerX, int playerY, int playerW, int playerH, int ObjectX, int ObjectY, int ObjectW, int objectH);

#endif // !_JUDGE_H