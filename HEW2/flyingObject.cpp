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
	if (flyingObject.type == FLYING_OBJECT_BLOCK) {
		DrawGameSprite(blockObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
	}
	if (flyingObject.type == FLYING_OBJECT_ENEMY) {
		DrawGameSprite(enemyObjectTextureId, flyingObject.trans.pos - D3DXVECTOR2(0.5, 0.5), 50);
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
			itr->trans.pos += itr->dir;

			itr->trans.UpdatePos();

			if (itr->trans.pos.x > MAPCHIP_WIDTH + FLYINGOBJECT_ADD_RANGE ||
				itr->trans.pos.x < -MAPCHIP_WIDTH - FLYINGOBJECT_ADD_RANGE ||
				itr->trans.pos.y > MAPCHIP_HEIGHT + FLYINGOBJECT_ADD_RANGE ||
				itr->trans.pos.y < -MAPCHIP_HEIGHT - FLYINGOBJECT_ADD_RANGE) {
				itr = flyingObjects.erase(itr);
			}
			else {
				itr++;
			}
		}
	}
}