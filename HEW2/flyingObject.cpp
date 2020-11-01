
#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"

static std::list<FlyingObject> flyingObjects;
static int flyingObjectTextureId = TEXTURE_INVALID_ID;

std::list<FlyingObject>* GetFlyingObjects(){
	return &flyingObjects;
}

void InitFlyingObject(){
	//flyingObjectTextureId = ReserveTextureLoadFile("");
}
void UninitFlyingObject(){

}
void DrawFlyingObject(){

	auto flyingObjects = *GetFlyingObjects();
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		DrawSprite(flyingObjectTextureId, itr->pos, 10);
	}
}
void UpdateFlyingObject(){
	auto flyingObjects = *GetFlyingObjects();
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		itr->pos += itr->dir;
	}
}