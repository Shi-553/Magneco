#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"
#include "map.h"
#include "time.h"
#include "npc.h"
#include "player.h"
#include "flyingObjectSponer.h"

// flyingObject描画範囲の加算分
#define FLYINGOBJECT_ADD_RANGE (5)


static std::list<FlyingObject> flyingObjects;
static bool existsUFO = false;
static int flyingObjectTextureIds[FLYING_OBJECT_MAX];
static int blockAnimationTextureId;

static SponeId spone;


std::list<FlyingObject>* GetFlyingObjects() {
	return &flyingObjects;
}

void AddFlyingObjects(FlyingObject* flyingObject) {
	if (flyingObject->type == FLYING_OBJECT_UFO) {// ufo画面上にない場合
		if (existsUFO) {
			return;
		}
		existsUFO = true;
	}
	flyingObjects.push_back(*flyingObject);

}

void InitFlyingObject() {
	flyingObjects.clear();
	flyingObjectTextureIds[FLYING_OBJECT_BLOCK] = ReserveTextureLoadFile("texture/block01.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY] = ReserveTextureLoadFile("texture/jellyalien01.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY_BREAK_BLOCK] = ReserveTextureLoadFile("texture/meteorite_1.png");
	flyingObjectTextureIds[FLYING_OBJECT_UFO] = ReserveTextureLoadFile("texture/ufo.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY_SECOND] = ReserveTextureLoadFile("texture/jellyaliengirl01.png");
	flyingObjectTextureIds[FLYING_OBJECT_PLAYER_BLOCK] = ReserveTextureLoadFile("texture/block03.png");
	flyingObjectTextureIds[FLYING_OBJECT_PURGE_BLOCK] = ReserveTextureLoadFile("texture/block03.png");
	existsUFO = false;

	flyingObjectTextureIds[FLYING_OBJECT_ITEM_ADD_SPEED] = ReserveTextureLoadFile("texture/hane.png");
	flyingObjectTextureIds[FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE] = ReserveTextureLoadFile("texture/maguneticPower.png");
	flyingObjectTextureIds[FLYING_OBJECT_CHECKPOINT_OFF] = ReserveTextureLoadFile("texture/checkpoint_off.png");
	blockAnimationTextureId = ReserveTextureLoadFile("texture/block_anime.png");

}
void UninitFlyingObject() {
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		ReleaseTexture(flyingObjectTextureIds[i]);
	}
	ReleaseTexture(blockAnimationTextureId);
}
void DrawFlyingObject(FlyingObject flyingObject) {
	Player* player = GetPlayer();
	auto textureId = flyingObjectTextureIds[flyingObject.type];


	if (flyingObject.isAnime) {
		DrawGameSprite(blockAnimationTextureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0, 50, flyingObject.size.ToD3DXVECTOR2(), { (float)(4 * player->putFrame / DEFAULT_PUT_REQUIRED_FRAME) * 32, 0 }, { 32, 32 });
		return;
	}
	DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0, 50, flyingObject.size.ToD3DXVECTOR2());
}

void DrawFlyingObject() {

	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		DrawFlyingObject(*itr);
	}
}
void UpdateFlyingObject() {
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
		if (UpdateFlyingObject(&*itr, 1)) {
			itr = flyingObjects.erase(itr);
		}
		else {
			itr++;
		}
	}
}
void BackFlyingObject(int frame) {
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
		if (UpdateFlyingObject(&*itr, -frame)) {
			if (itr->type == FLYING_OBJECT_UFO) {
				DestroyUFO();
			}
			itr = flyingObjects.erase(itr);
		}
		else {
			itr++;
		}
	}
}
bool UpdateFlyingObject(FlyingObject* flyingObject, float speed) {
	if (flyingObject->type == FLYING_OBJECT_UFO) {
		flyingObject->dir = (GetNpc()->trans.pos + ADD_UFO_POS) - flyingObject->trans.pos;

	}
	auto nomal = flyingObject->dir;
	D3DXVec2Normalize(&nomal, &nomal);
	flyingObject->trans.pos += nomal * speed * GetDeltaTime();

	flyingObject->trans.UpdatePos();

	if (flyingObject->trans.pos.x > GetMapWidth() + FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.x < -GetMapWidth() - FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.y > GetMapHeight() + FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.y < -GetMapHeight() - FLYINGOBJECT_ADD_RANGE) {
		return true;
	}
	else {
		return false;
	}

}

void DestroyUFO() {
	existsUFO = false;
}



bool IsFlyingObjectItem(FlyingObjectType type) {
	switch (type)
	{
	case FLYING_OBJECT_ITEM_ADD_SPEED:
	case FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE:
		//case FLYING_OBJECT_CHECKPOINT_OFF:
		return true;
	default:
		return false;
	}
}


int GetFlyingObjectTextureId(FlyingObjectType type) {
	if (type < FLYING_OBJECT_BLOCK || FLYING_OBJECT_MAX <= type) {
		return TEXTURE_INVALID_ID;
	}
	return flyingObjectTextureIds[(int)type];
}