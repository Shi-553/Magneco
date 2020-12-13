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
#include <math.h>

#define PLAYER_TEXTURE_WIDTH 32
#define PLAYER_TEXTURE_HEIGHT 32

static int textureId = TEXTURE_INVALID_ID;
static Player player;

static int  playerTextureVertical = 0;



void InitPlayer() {
	textureId = ReserveTextureLoadFile("texture/player_32×32.png");

	player.trans.Init(3.5, 3.5);
	player.flyingObjectList.clear();
	player.purgeFlyingObjectList.clear();
	player.dir = { 0,0 };
	player.speed = 6;
	player.frame = 0;
	playerTextureVertical = 0;
	player.blockMax = 4;
}

void UninitPlayer() {
	ReleaseTexture(textureId);
}

void UpdatePlayer() {
	if (player.dir.x != 0 || player.dir.y != 0) {
		if (fabsf(player.dir.y) > fabsf(player.dir.x)) {
			if (0 < player.dir.y) {
				playerTextureVertical = 0;
			}
			else {
				playerTextureVertical = PLAYER_TEXTURE_HEIGHT * 3;
			}
		}
		else {
			if (0 < player.dir.x) {
				playerTextureVertical = PLAYER_TEXTURE_HEIGHT * 2;
			}
			else {
				playerTextureVertical = PLAYER_TEXTURE_HEIGHT;
			}
		}
	}
	for (auto itr = player.purgeFlyingObjectList.begin(); itr != player.purgeFlyingObjectList.end(); ) {
		if (UpdateFlyingObject(&*itr, player.speed  / 2)) {
			itr = player.purgeFlyingObjectList.erase(itr);
		}
		else {
			itr++;
		}
	}
	auto dir = player.dir;
	D3DXVec2Normalize(&player.dir, &player.dir);


	auto last = player.trans.pos;
	auto move = one * player.speed  * GetDeltaTime();
	auto move = dir * player.speed * 60 * GetDeltaTime();

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
		itr->trans.pos += player.trans.pos - last;
		itr->trans.UpdatePos();
	}

	player.dir = { 0,0 };

	player.frame++;


}

void DrawPlayer() {
	for (auto itr = player.purgeFlyingObjectList.begin(); itr != player.purgeFlyingObjectList.end(); itr++) {
		DrawFlyingObject(*itr);
	}


	auto tPos = D3DXVECTOR2(
		PLAYER_TEXTURE_WIDTH * (player.frame / 16 % 4),
		playerTextureVertical
	);

	DrawGameSprite(textureId, player.trans.pos - D3DXVECTOR2(0.5, 0.5), 30, tPos, D3DXVECTOR2(PLAYER_TEXTURE_WIDTH, PLAYER_TEXTURE_HEIGHT));


	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		DrawFlyingObject(*itr);
	}
}



void RotateLeftPlayer() {

}

void RotateRightPlayer() {

}

void MoveUpPlayer() {
	player.dir.y--;
}

void MoveDownPlayer() {
	player.dir.y++;
}

void MoveLeftPlayer() {
	player.dir.x--;
}
void MoveRightPlayer() {
	player.dir.x++;
}


void MovePlayer(D3DXVECTOR2 dir) {
	player.dir = dir;
}

void BlockDecision() {
	bool canBlockPut = true;

	for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
		itr != player.flyingObjectList.end(); itr++) {
		MapType type;
		type = GetMapType(itr->trans.GetIntPos());
		if (type != MAP_BLOCK_NONE) {
			canBlockPut = false;
			break;
		}
	}
	if (canBlockPut == false) {
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

Player* GetPlayer() {
	return &player;
}

void PutBeacon() {
	auto mapType = GetMapType(player.trans.GetIntPos());
	if (mapType == MAP_BLOCK || mapType == MAP_GOAL) {
		UpdateNPCShortestPath(player.trans.GetIntPos());
	}
}

void PurgePlayerFlyingObject() {
	for (auto itr = player.flyingObjectList.begin(); itr != player.flyingObjectList.end();) {
		if (INTVECTOR2(player.dir) == INTVECTOR2(0, 0)) {
			itr->dir = itr->trans.pos - player.trans.pos;
		}
		else {
			itr->dir = player.dir;
		}
		itr->type = FLYING_OBJECT_PURGE_BLOCK;

		player.purgeFlyingObjectList.push_back(*itr);
		itr = player.flyingObjectList.erase(itr);
	}
}