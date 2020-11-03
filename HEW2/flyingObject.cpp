
#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"


static std::list<FlyingObject> flyingObjects;
static int blockObjectTextureId = TEXTURE_INVALID_ID;
static int enemyObjectTextureId = TEXTURE_INVALID_ID;
static int frame = 0;

std::list<FlyingObject>* GetFlyingObjects(){
	return &flyingObjects;
}

void InitFlyingObject(){
	frame = 0;
	blockObjectTextureId = ReserveTextureLoadFile("texture/FLYING_OBJECT_BLOCK.png");
	enemyObjectTextureId = ReserveTextureLoadFile("texture/FLYING_OBJECT_ENEMY.png");
}
void UninitFlyingObject(){

}
void DrawFlyingObject(FlyingObject flyingObject) {
	if (flyingObject.type == FLYING_OBJECT_BLOCK) {
		DrawGameSprite(blockObjectTextureId, flyingObject.pos, 50);
	}
	if (flyingObject.type == FLYING_OBJECT_ENEMY) {
		DrawGameSprite(enemyObjectTextureId, flyingObject.pos, 50);
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

			if (itr->pos.x > 10 || itr->pos.x < -10 || itr->pos.y>10 || itr->pos.y < -10) {
				itr = flyingObjects.erase(itr);
			}
			else {
				itr++;
			}
		}
	}
}