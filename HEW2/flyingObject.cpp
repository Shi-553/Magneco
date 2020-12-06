#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"
#include "map.h"
#include "time.h"

// flyingObject•`‰æ”ÍˆÍ‚Ì‰ÁŽZ•ª
#define FLYINGOBJECT_ADD_RANGE (5)


static std::list<FlyingObject> flyingObjects;
static int blockObjectTextureId = TEXTURE_INVALID_ID;
static int enemyObjectTextureId = TEXTURE_INVALID_ID;
static int purgeBlockObjectTextureId = TEXTURE_INVALID_ID;

std::list<FlyingObject>* GetFlyingObjects() {
	return &flyingObjects;
}

void AddFlyingObjects(FlyingObject* flyingObject) {
	flyingObjects.push_back(*flyingObject);
}

void InitFlyingObject() {
	flyingObjects.clear();
	blockObjectTextureId = ReserveTextureLoadFile("texture/block01.png");
	enemyObjectTextureId = ReserveTextureLoadFile("texture/jellyalien01.png");
	purgeBlockObjectTextureId = ReserveTextureLoadFile("texture/block03.png");
}
void UninitFlyingObject() {
	ReleaseTexture(blockObjectTextureId);
	ReleaseTexture(enemyObjectTextureId);
	ReleaseTexture(purgeBlockObjectTextureId);
}
void DrawFlyingObject(FlyingObject flyingObject) {
	if (flyingObject.type == FLYING_OBJECT_BLOCK) {
		DrawGameSprite(blockObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
	}
	if (flyingObject.type == FLYING_OBJECT_ENEMY) {
		DrawGameSprite(enemyObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
	}
	if (flyingObject.type == FLYING_OBJECT_PURGE_BLOCK) {
		DrawGameSprite(purgeBlockObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
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
	auto nomal = flyingObject->dir;
	D3DXVec2Normalize(&nomal, &nomal);
	flyingObject->trans.pos += nomal* speed * GetDeltaTime();

	flyingObject->trans.UpdatePos();

	if (flyingObject->trans.pos.x > MAPCHIP_WIDTH + FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.x < -MAPCHIP_WIDTH - FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.y > MAPCHIP_HEIGHT + FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.y < -MAPCHIP_HEIGHT - FLYINGOBJECT_ADD_RANGE) {
		return true;
	}
	else {
		return false;
	}

}