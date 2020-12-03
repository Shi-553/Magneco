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
#include "time.h"

#define PLAYER_TEXTURE_WIDTH 32
#define PLAYER_TEXTURE_HEIGHT 32

static int textureId = TEXTURE_INVALID_ID;
static Player player;

static int  playerTextureVertical = 0;



void InitPlayer(){
	textureId = ReserveTextureLoadFile("texture/player_32Å~32.png");

	player.trans.Init(3.5, 3.5);
	player.flyingObjectList.clear();
	player.dir = {0,0};
	player.speed = 0.1;
	player.frame = 0;
	playerTextureVertical = 0;
}

void UninitPlayer(){
	ReleaseTexture(textureId);
}

void UpdatePlayer(){
	D3DXVECTOR2 one = player.dir.ToD3DXVECTOR2();
	D3DXVec2Normalize(&one, &one);
	

	auto last = player.trans.pos;
	auto move = one * player.speed * 60 * GetDeltaTime();

	player.trans.pos.x += move.x;
	auto mapType = GetMapType(INTVECTOR2(player.trans.pos));
	if (mapType == MAP_NONE || mapType == MAP_WALL) {
		player.trans.pos.x = last.x;
	}

	player.trans.pos.y += move.y;
	 mapType = GetMapType(INTVECTOR2(player.trans.pos));
	if (mapType == MAP_NONE || mapType == MAP_WALL) {
		player.trans.pos.y = last.y;
	}

	player.trans.UpdatePos();

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		itr->trans.pos +=  player.trans.pos-last;
		itr->trans.UpdatePos();
	}

	player.dir = { 0,0 };

	player.frame++;

}

void DrawPlayer(){


	auto tPos = D3DXVECTOR2(
		PLAYER_TEXTURE_WIDTH * (player.frame / 16 % 4),
		playerTextureVertical
			);

			DrawGameSprite(textureId, player.trans.pos - D3DXVECTOR2(0.5, 0.5), 30, tPos, D3DXVECTOR2(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));


    for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
	itr != player.flyingObjectList.end(); itr++){
	DrawFlyingObject(*itr);
    }
}



void RotateLeftPlayer(){

}

void RotateRightPlayer(){

}

void MoveUpPlayer(){
	player.dir.y--;
	playerTextureVertical = PLAYER_TEXTURE_HEIGHT * 3;
}

void MoveDownPlayer(){
	player.dir.y++;
	playerTextureVertical = 0;
}

void MoveLeftPlayer(){
	player.dir.x--;
	playerTextureVertical = PLAYER_TEXTURE_HEIGHT;
}

void MoveRightPlayer(){
	player.dir.x++;
	playerTextureVertical = PLAYER_TEXTURE_HEIGHT * 2;
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
		UpdateNPCShortestPath(player.trans.GetIntPos());
	}
}

void PurgePlayerFlyingObject() {
	player.flyingObjectList.clear();
}