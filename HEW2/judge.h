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

bool JudgePlayerandBlockHit();
bool JudgeBlockandEnemyHit();
bool JudgePlayerandEnemyHit();

#endif // !_JUDGE_H