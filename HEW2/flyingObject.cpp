#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"
#include "map.h"

// flyingObject•`‰æ”ÍˆÍ‚Ì‰ÁŽZ•ª
#define FLYINGOBJECT_ADD_RANGE (5)


static std::list<FlyingObject> flyingObjects;
static int blockObjectTextureId = TEXTURE_INVALID_ID;
static int enemyObjectTextureId = TEXTURE_INVALID_ID;
static int frame = 0;

std::list<FlyingObject>* GetFlyingObjects(){
	return &flyingObjects;
}

void AddFlyingObjects(FlyingObject* flyingObject) {
	flyingObject->lastPos = flyingObject->pos-flyingObject->dir;
	flyingObjects.push_back(*flyingObject);
}

void InitFlyingObject(){
	flyingObjects.clear();
	frame = 0;
	blockObjectTextureId = ReserveTextureLoadFile("texture/block01.png");
	enemyObjectTextureId = ReserveTextureLoadFile("texture/jellyalien01.png");
}
void UninitFlyingObject(){
	ReleaseTexture(blockObjectTextureId);
	ReleaseTexture(enemyObjectTextureId);
}
void DrawFlyingObject(FlyingObject flyingObject) {
	auto intPos = D3DXVECTOR2((int)flyingObject.pos.x, (int)flyingObject.pos.y);
	if (flyingObject.type == FLYING_OBJECT_BLOCK) {
		DrawGameSprite(blockObjectTextureId, intPos, 50);
	}
	if (flyingObject.type == FLYING_OBJECT_ENEMY) {
		DrawGameSprite(enemyObjectTextureId, intPos, 50);
	}
}
void DrawFlyingObject(){

	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		DrawFlyingObject(*itr);
	}
}
void UpdateFlyingObject(){
	frame++;
	if (frame > 20) {
		frame = 0;

		for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
			itr->lastPos = itr->pos;
			itr->pos += itr->dir;

			if (itr->pos.x > MAPCHIP_WIDTH + FLYINGOBJECT_ADD_RANGE ||
				itr->pos.x < -MAPCHIP_WIDTH - FLYINGOBJECT_ADD_RANGE ||
				itr->pos.y > MAPCHIP_HEIGHT + FLYINGOBJECT_ADD_RANGE ||
				itr->pos.y < -MAPCHIP_HEIGHT - FLYINGOBJECT_ADD_RANGE) {
				itr = flyingObjects.erase(itr);
			}
			else {
				itr++;
			}
		}
	}
}