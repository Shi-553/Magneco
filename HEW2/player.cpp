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
#include "judge.h"
#include "sound.h"

#define PLAYER_TEXTURE_WIDTH 32
#define PLAYER_TEXTURE_HEIGHT 32

#define PLAYER_PURGE_SPEED 6

static int textureId = TEXTURE_INVALID_ID;
static Player player;

static int  playerTextureVertical = 0;

static int putPredictionTextureId = TEXTURE_INVALID_ID;

void BlockDecision();
void ToFreeFlyingObject(FlyingObject& flyingObject);
void UpdatePutFlyingObject();
bool IsOverlaped(FlyingObject& flyingObject);

void InitPlayer() {
	textureId = ReserveTextureLoadFile("texture/player_32×32.png");
	putPredictionTextureId = ReserveTextureLoadFile("texture/putPrediction.png");

	player.trans.Init(3.5, 3.5);
	player.flyingObjectList.clear();
	player.purgeFlyingObjectList.clear();
	player.putFlyingObjectList.clear();
	player.dir = { 0,0 };
	player.baseSpeed = 6;
	player.addSpeed = 0;
	player.frame = 0;
	playerTextureVertical = 0;
	player.blockMax = 4;
	player.checkCheckpoint = false;
	player.isPut = false;
	player.putFrame = 0;
	player.stanTime = 0;
	player.invicibleTime = 0;
	player.size = { 1,1 };
}

void UninitPlayer() {
	ReleaseTexture(textureId);
	ReleaseTexture(putPredictionTextureId);
}

void UpdatePlayer() {
	if (player.invicibleTime > 0) {
		player.invicibleTime--;
	}

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
	auto speed = player.baseSpeed - (player.baseSpeed / 2 * player.flyingObjectList.size() / player.blockMax) + player.addSpeed;

	for (auto itr = player.purgeFlyingObjectList.begin(); itr != player.purgeFlyingObjectList.end(); ) {
		if (UpdateFlyingObject(&*itr, PLAYER_PURGE_SPEED)) {
			itr = player.purgeFlyingObjectList.erase(itr);
		}
		else {
			itr++;
		}
	}



	if (player.stanTime == 0) {
		auto length = D3DXVec2Length(&player.dir);
		if (length > 1.0f) {
			player.dir /= length;
		}
		auto last = player.trans.pos;
		auto move = player.dir * speed * GetDeltaTime();


		player.trans.pos += move;


		player.trans.UpdatePos();

		for (std::list<FlyingObject>::iterator itr = player.flyingObjectList.begin();
			itr != player.flyingObjectList.end(); itr++) {
			itr->trans.pos += player.trans.pos - last;
			itr->trans.UpdatePos();
		}

		// ブロックを置く処理
		if (player.isPut) {
			player.putFrame++;
		}
		if (player.putFrame >= DEFAULT_PUT_REQUIRED_FRAME) {
			BlockDecision();
		}


		player.frame++;

	}
	else {

		player.stanTime--;
	}

	player.dir = { 0,0 };

	UpdatePutFlyingObject();
}

void DrawPlayer() {

	for (auto& v : player.putFlyingObjectList) {
		DrawGameSprite(putPredictionTextureId, v.trans.GetIntPos().ToD3DXVECTOR2(), 1);
	}

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

void UpdatePutFlyingObject() {
	player.putFlyingObjectList.clear();

	if (player.flyingObjectList.empty()) {
		return;
	}
	auto& front = player.flyingObjectList.front();
	if (front.type == FLYING_OBJECT_CHECKPOINT_OFF && GetMapType(front.trans.GetIntPos()) == MAP_BLOCK_NONE) {
		player.putFlyingObjectList.push_back(front);
		return;
	}

	for (auto& f : player.flyingObjectList) {
		f.isAnime = false;
	}

	bool isAdd = false;

	while (true) {
		isAdd = false;

		std::list<FlyingObject> modosu;

		while (!player.flyingObjectList.empty()) {
			auto& current = player.flyingObjectList.front();


			if (GetMapType(current.trans.GetIntPos()) == MAP_BLOCK_NONE && MapFourDirectionsJudgment(current.trans.GetIntPos()) &&
				std::find_if(player.putFlyingObjectList.begin(), player.putFlyingObjectList.end(), [&current](FlyingObject f) {return f.uid == current.uid; }) == player.putFlyingObjectList.end()) {
				MapChange(current);

				player.putFlyingObjectList.push_back(current);
				if (player.isPut) {
					current.isAnime = true;
				}
				isAdd = true;
				modosu.push_back(current);
				player.flyingObjectList.pop_front();
				break;
			}
			else {
				modosu.push_back(current);
				player.flyingObjectList.pop_front();
			}
		}

		for (auto itrM = modosu.begin(); itrM != modosu.end(); itrM++) {
			player.flyingObjectList.push_back(*itrM);
		}

		if (!isAdd) {
			break;
		}
	}

	for (auto& v : player.putFlyingObjectList) {
		auto m = GetMap(v.trans.GetIntPos());
		if (m != NULL) {
			m->type = MAP_BLOCK_NONE;
		}

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
	if (player.flyingObjectList.empty()) {
		return;
	}
	player.isPut = false;
	player.putFrame = 0;


	for (auto itr = player.flyingObjectList.begin(); itr != player.flyingObjectList.end(); ) {
		if (itr->isAnime) {
			itr->isAnime = false;
			MapChange(*itr);
			itr = player.flyingObjectList.erase(itr);
			continue;
		}
		itr++;
	}

	RemoteBlockToFreeFlyingObject();
}

Player* GetPlayer() {
	return &player;
}

void PutBeacon() {

	auto mapType = GetMapType(player.trans.GetIntPos());
	if (CanGoNPCMapType(mapType)) {
		UpdateNPCShortestPath(player.trans.GetIntPos());
	}
}

void PurgePlayerFlyingObject() {
	if (player.checkCheckpoint)
	{
		return;
	}
	for (auto itr = player.flyingObjectList.begin(); itr != player.flyingObjectList.end();) {
		itr->dir = itr->trans.pos - player.trans.pos;
		itr->type = FLYING_OBJECT_PURGE_BLOCK;

		player.purgeFlyingObjectList.push_back(*itr);
		itr = player.flyingObjectList.erase(itr);
	}
}
bool PlayerExport(FILE* fp) {

	D3DXVECTOR2 pos = player.trans.pos;
	//	ファイルへの書き込み処理
	fwrite(&pos, sizeof(D3DXVECTOR2), 1, fp);

	return true;
}


bool PlayerImport(FILE* fp) {
	D3DXVECTOR2 pos;
	//	ファイルへの読み込み処理
	fread(&pos, sizeof(D3DXVECTOR2), 1, fp);
	player.trans.Init(pos);

	return true;
}

void MakePut() {
	if (player.flyingObjectList.empty()) {
		return;
	}

	auto& front = player.flyingObjectList.front();
	if (front.type == FLYING_OBJECT_CHECKPOINT_OFF) {
		if (GetMapType(front.trans.GetIntPos()) != MAP_BLOCK_NONE) {
			return;
		}
		MapChange(front);
		player.flyingObjectList.clear();
		player.invicibleTime = 0;
		player.checkCheckpoint = false;
		return;
	}

	player.isPut = true;
}

void PutCansel() {
	player.isPut = false;
	player.putFrame = 0;

	for (auto itr = player.flyingObjectList.begin(); itr != player.flyingObjectList.end(); itr++) {
		itr->isAnime = false;
	}
}


bool GetBlock(FlyingObject& itr, D3DXVECTOR2& attachPos) {
	if (player.flyingObjectList.size() >= player.blockMax || player.checkCheckpoint || player.isPut) {
		return false;
	}

	if (player.flyingObjectList.size() > 0 && itr.type == FLYING_OBJECT_CHECKPOINT_OFF) {
		return false;
	}

	auto dir = itr.trans.pos - attachPos;

	if (fabsf(dir.x) > fabsf(dir.y)) {
		dir.x = dir.x > 0 ? 1 : -1;
		dir.y = 0;
	}
	else {
		dir.y = dir.y > 0 ? 1 : -1;
		dir.x = 0;
	}

	auto itrPos = itr.trans.pos;

	auto pos = attachPos + dir;
	itr.trans.Init(pos);

	if (IsOverlaped(itr)) {
		itr.trans.Init(itrPos);

		auto pToF = itr.trans.pos - player.trans.pos;
		bool isX = fabsf(pToF.x) > fabsf(pToF.y);

		pToF.x = pToF.x > 0 ? 1 : -1;
		pToF.y = pToF.y > 0 ? 1 : -1;

		while (IsOverlaped(itr)) {
			pos = itr.trans.pos;
			if (isX) {
				pos.x += pToF.x;
			}
			else {
				pos.y += pToF.y;
			}
			isX = !isX;

			itr.trans.Init(pos);

		} 
	}
	itr.trans.pos = itr.trans.GetIntPos().ToD3DXVECTOR2() + player.trans.pos-player.trans.GetIntPos().ToD3DXVECTOR2();

	if (itr.type == FLYING_OBJECT_CHECKPOINT_OFF) {

		player.checkCheckpoint = true;
		player.invicibleTime = -1;
	}
	else
	{
		itr.type = FLYING_OBJECT_PLAYER_BLOCK;
	}


	player.flyingObjectList.push_back(itr);
	return true;
}
bool IsOverlaped(FlyingObject& flyingObject) {
	return flyingObject.trans.GetIntPos() == player.trans.GetIntPos() ||
		find_if(player.flyingObjectList.begin(), player.flyingObjectList.end(), [&flyingObject](FlyingObject f) {
		return f.trans.GetIntPos() == flyingObject.trans.GetIntPos();
			}) != player.flyingObjectList.end();
}

bool IsPlayerInvicible() {
	return player.invicibleTime > 0 || player.invicibleTime == -1;
}



bool DamagePlayer() {
	if (IsPlayerInvicible()) {
		return false;
	}
	for (auto itr = player.flyingObjectList.begin(); itr != player.flyingObjectList.end();) {
		ToFreeFlyingObject(*itr);
		itr = player.flyingObjectList.erase(itr);
	}
	player.checkCheckpoint = false;

	PlaySound(SOUND_LEBEL_SE_STAN);
	player.stanTime = DEFAULT_PLAYER_STAN_FRAME;
	player.invicibleTime = DEFAULT_PLAYER_INVICIBLE_FRAME;
	return true;
}



bool RemoteBlockToFreeFlyingObject() {
	list<INTVECTOR2> okPoss;
	okPoss.push_back(player.trans.GetIntPos());

	list<int> toFreeFlyingObjectIds;
	for (auto itr = player.flyingObjectList.begin(); itr != player.flyingObjectList.end(); itr++) {
		toFreeFlyingObjectIds.push_back(itr->id);
	}

	while (true) {
		bool isModosita = false;

		for (auto idItr = toFreeFlyingObjectIds.begin(); idItr != toFreeFlyingObjectIds.end(); idItr++) {
			auto itr = find_if(player.flyingObjectList.begin(), player.flyingObjectList.end(), [idItr](FlyingObject f) {return f.id == *idItr; });
			if (itr == player.flyingObjectList.end()) {
				continue;
			}
			auto& pos = itr->trans.GetIntPos();
			auto& left = pos + INTVECTOR2(-1, 0);
			auto& right = pos + INTVECTOR2(1, 0);
			auto& top = pos + INTVECTOR2(0, -1);
			auto& bottom = pos + INTVECTOR2(0, 1);
			if (find(okPoss.begin(), okPoss.end(), left) != okPoss.end() ||
				find(okPoss.begin(), okPoss.end(), right) != okPoss.end() ||
				find(okPoss.begin(), okPoss.end(), top) != okPoss.end() ||
				find(okPoss.begin(), okPoss.end(), bottom) != okPoss.end()) {

				okPoss.push_back(pos);
				toFreeFlyingObjectIds.erase(idItr);
				isModosita = true;
				break;
			}

		}
		if (!isModosita) {
			break;
		}
	}

	for (auto idItr = toFreeFlyingObjectIds.begin(); idItr != toFreeFlyingObjectIds.end(); ) {
		auto itr = find_if(player.flyingObjectList.begin(), player.flyingObjectList.end(), [idItr](FlyingObject f) {return f.id == *idItr; });
		if (itr == player.flyingObjectList.end()) {
			idItr = toFreeFlyingObjectIds.erase(idItr);
			continue;
		}
		idItr++;
		ToFreeFlyingObject(*itr);
		player.flyingObjectList.erase(itr);
	}
	return !toFreeFlyingObjectIds.empty();
}
void ToFreeFlyingObject(FlyingObject& flyingObject) {
	flyingObject.dir = (flyingObject.trans.GetIntPos() - player.trans.GetIntPos()).ToD3DXVECTOR2();
	flyingObject.type = FLYING_OBJECT_BLOCK;
	flyingObject.speed = 3;
	flyingObject.isAnime = false;
	AddFlyingObjects(&flyingObject);
}