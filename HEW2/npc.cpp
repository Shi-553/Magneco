#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "myd3d.h"
#include "gameSrite.h"
#include "npc.h"
#include <d3dx9.h>


static int NPCTextureId = TEXTURE_INVALID_ID;
static NPC npc;

void InitNPC(){
	NPCTextureId = ReserveTextureLoadFile("texture/npc.png");
	npc.NPCPosition = D3DXVECTOR2(2, 7);
}

void UninitNPC() {
	ReleaseTexture(NPCTextureId);
}

void UpdateNPC() {

}

void DrawNPC() {
	D3DXVECTOR2 intposition;
	intposition = D3DXVECTOR2((int)npc.NPCPosition.x, (int)npc.NPCPosition.y);
	DrawGameSprite(NPCTextureId, intposition, 30);

}
