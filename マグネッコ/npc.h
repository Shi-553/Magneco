#ifndef _NPC_H
#define _NPC_H
#include "trans.h"

typedef struct NPC_tag {
	TRANS trans;
	float speed;
	int frame;
	int aniFrame;
	bool isMove;
	bool contactUFO;
	int takeOutFrame;
	INTVECTOR2 responePos;
	D3DXVECTOR2 size;
	int fallFrame;
	int respawnFrame;
}NPC;

#define TAKE_OUT_FRAME_LIMIT (10 * 60)

void InitNPC();
void UpdateNPC();
void DrawNPC();
void UninitNPC();

bool UpdateNPCShortestPath(INTVECTOR2 beaconPos);
bool UpdateNPCShortestPath();

void SecureMapLabelList();

NPC* GetNpc();

bool NPCExport(FILE* fp);
bool NPCImport(FILE* fp);

void SetNPCResponePos(INTVECTOR2 pos);

void NPCContactUFO();
void NPCDeleteUFO();
#endif // !_NPC_H