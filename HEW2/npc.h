#ifndef _NPC_H
#define _NPC_H

typedef struct NPC_tag {
	D3DXVECTOR2 NPCPosition;
}NPC;


void InitNPC();
void UpdateNPC();
void DrawNPC();
void UninitNPC();

#endif // !_NPC_H