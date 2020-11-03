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

static float playerSpeed = 0.1f;
static int textureId = TEXTURE_INVALID_ID;
static Player player;

void InitPlayer(){
	textureId = ReserveTextureLoadFile("texture/player.png");

	player.position = D3DXVECTOR2(3, 3);
	player.flyingObjectList.clear();
}

void UninitPlayer(){
	ReleaseTexture(textureId);
}

void UpdatePlayer(){

}

void DrawPlayer(){
	D3DXVECTOR2 intposition;
	intposition = D3DXVECTOR2((int)player.position.x, (int)player.position.y);
	DrawGameSprite(textureId,intposition,30);
}


void RotateLeftPlayer(){

}

void RotateRightPlayer(){

}

void MoveUpPlayer(){
	player.position.y += -playerSpeed;
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.y += -playerSpeed;
	}
}

void MoveDownPlayer(){
	player.position.y += playerSpeed;
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.y += playerSpeed;
	}
}

void MoveLeftPlayer(){
	player.position.x += -playerSpeed;
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.x += -playerSpeed;
	}
}

void MoveRightPlayer(){
	player.position.x += playerSpeed;
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
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		MapChange(*itr);
	}
	player.flyingObjectList.clear();
}

Player* GetPlayer(){
return &player;
}
