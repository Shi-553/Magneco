#ifndef _NPC_H
#define _NPC_H
#include "trans.h"

typedef struct NPC_tag {
	TRANS trans;
	float speed;
	int frame;
	int aniFrame;
	bool isMove;
}NPC;


void InitNPC();
void UpdateNPC();
void DrawNPC();
void UninitNPC();

void UpdateNPCShortestPath(INTVECTOR2 beaconPos);
void UpdateNPCShortestPath();

void SecureMapLabelList();

#endif // !_NPC_H