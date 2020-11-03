
#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"


static std::list<FlyingObject> flyingObjects;
static int flyingObjectTextureId = TEXTURE_INVALID_ID;
static int frame = 0;

std::list<FlyingObject>* GetFlyingObjects(){
	return &flyingObjects;
}

void InitFlyingObject(){
	frame = 0;
	flyingObjectTextureId = ReserveTextureLoadFile("texture/fade.png");
}
void UninitFlyingObject(){

}
void DrawFlyingObject(){

	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		DrawGameSprite(flyingObjectTextureId, itr->pos,50);
	}
}
void UpdateFlyingObject(){
	frame++;
	if (frame > 100) {
		frame = 0;

		for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
			itr->pos += itr->dir;
			if (itr->pos.x > 10 || itr->pos.x < -10 || itr->pos.y>10 || itr->pos.y < 10) {
				itr = flyingObjects.erase(itr);
			}
			else {
				itr++;
			}
		}
	}
}