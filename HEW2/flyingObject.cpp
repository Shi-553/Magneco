#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"
#include "map.h"
#include "time.h"
#include "npc.h"

// flyingObject描画範囲の加算分
#define FLYINGOBJECT_ADD_RANGE (5)


static std::list<FlyingObject> flyingObjects;
static int blockObjectTextureId = TEXTURE_INVALID_ID;
static int enemyObjectTextureId = TEXTURE_INVALID_ID;
static int anotherBlockObjectTextureId = TEXTURE_INVALID_ID;
static int ufoObjectTextureId = TEXTURE_INVALID_ID;
bool existsUFO = false;


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
	blockObjectTextureId = ReserveTextureLoadFile("texture/block01.png");
	enemyObjectTextureId = ReserveTextureLoadFile("texture/jellyalien01.png");
	anotherBlockObjectTextureId = ReserveTextureLoadFile("texture/block03.png");
	ufoObjectTextureId = ReserveTextureLoadFile("texture/ufo.png");
	existsUFO = false;
}
void UninitFlyingObject() {
	ReleaseTexture(blockObjectTextureId);
	ReleaseTexture(enemyObjectTextureId);
	ReleaseTexture(anotherBlockObjectTextureId);
	ReleaseTexture(ufoObjectTextureId);
}
void DrawFlyingObject(FlyingObject flyingObject) {
	if (flyingObject.type == FLYING_OBJECT_BLOCK) {
		DrawGameSprite(blockObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
	}
	if (flyingObject.type == FLYING_OBJECT_ENEMY) {
		DrawGameSprite(enemyObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
	}
	if (flyingObject.type == FLYING_OBJECT_UFO) {
		DrawGameSprite(ufoObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
	}
	if (flyingObject.type == FLYING_OBJECT_PLAYER_BLOCK ||flyingObject.type == FLYING_OBJECT_PURGE_BLOCK) {
		DrawGameSprite(anotherBlockObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
	}
}
void DrawFlyingObject() {

	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		DrawFlyingObject(*itr);
	}
}
void UpdateFlyingObject() {
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
		if (UpdateFlyingObject(&*itr,1)) {
			itr = flyingObjects.erase(itr);
		}
		else {
			itr++;
		}
	}
}
bool UpdateFlyingObject(FlyingObject* flyingObject,float speed) {
	if (flyingObject->type == FLYING_OBJECT_UFO) {
		flyingObject->dir = (GetNPC()->trans.pos + ADD_UFO_POS) - flyingObject->trans.pos;

	}
	auto nomal = flyingObject->dir;
	D3DXVec2Normalize(&nomal, &nomal);
	flyingObject->trans.pos += nomal* speed * GetDeltaTime();

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