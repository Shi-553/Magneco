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

static float playerSpeed = 1.0f;
static int textureId = TEXTURE_INVALID_ID;
static Player player;

void InitPlayer(){
	textureId = ReserveTextureLoadFile("texture/player.png");

	player.trans.Init(3, 3);
	player.flyingObjectList.clear();
	player.isMove = false;
}

void UninitPlayer(){
	ReleaseTexture(textureId);
}

void UpdatePlayer(){
	player.isMove = false;

}

void DrawPlayer(){
	DrawGameSprite(textureId, player.trans.GetIntPos().ToD3DXVECTOR2(), 30);

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
	player.isMove = true;

	player.trans.pos.y += -playerSpeed;
	player.trans.UpdatePos();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->trans.pos.y += -playerSpeed;
		itr->trans.UpdatePos();
	}
}

void MoveDownPlayer(){
	player.isMove = true;

	player.trans.pos.y += playerSpeed;
	player.trans.UpdatePos();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->trans.pos.y += playerSpeed;
		itr->trans.UpdatePos();
	}
}

void MoveLeftPlayer(){
	player.isMove = true;

	player.trans.pos.x += -playerSpeed;
	player.trans.UpdatePos();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->trans.pos.x += -playerSpeed;
		itr->trans.UpdatePos();
	}
}

void MoveRightPlayer(){
	player.isMove = true;

	player.trans.pos.x += playerSpeed;
	player.trans.UpdatePos();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->trans.pos.x += playerSpeed;
		itr->trans.UpdatePos();
	}
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