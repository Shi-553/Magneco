#ifndef _NPC_H
#define _NPC_H

typedef struct NPC_tag {
	D3DXVECTOR2 pos;
	float speed;
}NPC;


void InitNPC();
void UpdateNPC();
void DrawNPC();
void UninitNPC();

void UpdateNPCShortestPath(D3DXVECTOR2 beaconPos);
void UpdateNPCShortestPath();

#endif // !_NPC_H