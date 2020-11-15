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
}

void UninitPlayer(){
	ReleaseTexture(textureId);
}

void UpdatePlayer(){

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
	player.trans.pos.y += -playerSpeed;
	player.trans.UpdateY();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.y += -playerSpeed;
	}
}

void MoveDownPlayer(){
	player.trans.pos.y += playerSpeed;
	player.trans.UpdateY();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.y += playerSpeed;
	}
}

void MoveLeftPlayer(){
	player.trans.pos.x += -playerSpeed;
	player.trans.UpdateX();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.x += -playerSpeed;
	}
}

void MoveRightPlayer(){
	player.trans.pos.x += playerSpeed;
	player.trans.UpdateX();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.x += playerSpeed;
	}
}

void BlockDecision() {
	bool canBlockPut = true;

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		MapType type;
		type = GetMapType(itr->pos);
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
		if (MapFourDirectionsJudgment(itr->pos)) {
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
	auto mapType = GetMapType(player.trans.pos);
	if (mapType == MAP_BLOCK || mapType == MAP_GOAL) {
		UpdateNPCShortestPath(player.trans.pos);
	}
}