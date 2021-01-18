#ifndef _JUDGE_H
#define _JUDGE_H


void InitJudge();
void UninitJudge();
void UpdateJudge();
void DrawJudge();

void JudgePlayerandFlyingObjectHit();

template <typename P1, typename P2, typename S1, typename S2>
bool CheckBlockBlock(P1& pos1, P2& pos2, S1& size1, S2& size2) {
	auto mini = 0.1;

	auto left1 = pos1.x - (size1.x-mini) / 2.0;
	auto top1 = pos1.y - (size1.y-mini) / 2.0; // 1.5, 1.5 
	auto right1 = pos1.x + (size1.x-mini) / 2.0;
	auto bottom1 = pos1.y + (size1.y-mini) / 2.0; // 2.5, 2.5

	auto left2 = pos2.x - (size2.x-mini) / 2.0;
	auto top2 = pos2.y - (size2.y-mini) / 2.0; // 1.5, 1.5 
	auto right2 = pos2.x + (size2.x-mini) / 2.0;
	auto bottom2 = pos2.y + (size2.y- mini) / 2.0; // 2.5, 2.5

	if (right1 > left2 && left1 < right2) {
		if (bottom1 > top2 && top1 < bottom2) {
			return true;
		}
	}
	return false;
}
bool CheckCollision(std::list<FlyingObject>* flyingObjectList, FlyingObject& flyingObject);

#endif // !_JUDGE_H