#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "flyingObject.h"
#include "myd3d.h"
#include <list>
#include "player.h"
#include "game.h"
#include "map.h"
#include "gameSrite.h"
#include "npc.h"
#include "debugPrintf.h"

static int textureId = TEXTURE_INVALID_ID;
static Player player;

void InitPlayer(){
	textureId = ReserveTextureLoadFile("texture/player.png");

	player.trans.Init(3.5, 3.5);
	player.flyingObjectList.clear();
	player.dir = {0,0};
	player.speed = 0.1;
}

void UninitPlayer(){
	ReleaseTexture(textureId);
}

void UpdatePlayer(){
	D3DXVECTOR2 one = player.dir.ToD3DXVECTOR2();
	D3DXVec2Normalize(&one, &one);
	
	player.trans.pos += one *player.speed;
	player.trans.UpdatePos();
	DebugPrintf("{%d,%d}\n", player.trans.GetIntPos().x, player.trans.GetIntPos().y);
	DebugPrintf("{%f,%f}\n",player.trans.pos.x,player.trans.pos.y);
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->trans.pos +=  one *player.speed;
		itr->trans.UpdatePos();
	}

	player.dir = { 0,0 };

}

void DrawPlayer(){
	DrawGameSprite(textureId, player.trans.pos-D3DXVECTOR2(0.5,0.5), 30);

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		DrawFlyingObject(*itr);
	}
}


void RotateLeftPlayer(){

}

void RotateRightPlayer(){

}

void MoveUpPlayer(){
	player.dir.y--;
}

void MoveDownPlayer(){
	player.dir.y++;
}

void MoveLeftPlayer(){
	player.dir.x--;
}

void MoveRightPlayer(){
	player.dir.x++;
}

void BlockDecision() {
	bool canBlockPut = true;

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		MapType type;
		type = GetMapType(itr->trans.GetIntPos());
		if (type != MAP_BLOCK_NONE){
			canBlockPut = false;
			break;
		}
	}
	if (canBlockPut == false){
		return;
	}

	bool isFourDirections = false;

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		if (MapFourDirectionsJudgment(itr->trans.GetIntPos())) {
			isFourDirections = true;
			break;
		}
	}
	if (isFourDirections == false) {
		return;
	}

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		MapChange(*itr);
	}
	player.flyingObjectList.clear();

	UpdateNPCShortestPath();
}

Player* GetPlayer(){
return &player;
}

void PutBeacon() {
	auto mapType = GetMapType(player.trans.GetIntPos());
	if (mapType == MAP_BLOCK || mapType == MAP_GOAL) {
		UpdateNPCShortestPath(player.trans.pos);
	}
}