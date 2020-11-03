
#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"


static std::list<FlyingObject> flyingObjects;
static int flyingObjectTextureId = TEXTURE_INVALID_ID;

std::list<FlyingObject>* GetFlyingObjects(){
	return &flyingObjects;
}

void InitFlyingObject(){
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
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		itr->pos += itr->dir;
	}
}