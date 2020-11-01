#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "flyingObject.h"
#include "myd3d.h"
#include <list>
#include "player.h"
#include "game.h"
#include "map.h"

typedef struct Player_tag{
	D3DXVECTOR2 position;
	std::list < FlyingObject > flyingObjectList;
}Player;

static int TextureId = TEXTURE_INVALID_ID;
static Player player;

void InitPlayer(){
	player.position = D3DXVECTOR2(3, 3);
	player.flyingObjectList.clear();
}

void UninitPlayer(){

}

void UpdatePlayer(){

}

void DrawPlayer(){

}


void RotateLeftPlayer(){

}

void RotateRightPlayer(){

}

void MoveUpPlayer(){
	player.position.y += -1.0f;
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.y += -1.0;
	}
}

void MoveDownPlayer(){
	player.position.y += 1.0f;
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.y += 1.0;
	}
}

void MoveLeftPlayer(){
	player.position.x += -1.0f;
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.x += -1.0;
	}
}

void MoveRightPlayer(){
	player.position.x += 1.0f;
	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->pos.x += 1.0;
	}
}

void BlockDecision() {
	bool canBlockPut = true;

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		MapType type;
		type = GetMapType(itr->pos);
		if (type != BLOCK_NONE){
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
