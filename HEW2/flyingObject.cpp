
#include <list>
#include "flyingObject.h"
#include "texture.h"

static std::list<FlyingObject> flyingObjects;
static int flyingObjectTextureId = TEXTURE_INVALID_ID;

std::list<FlyingObject>* GetFlyingObjects(){
	return &flyingObjects;
}

void InitFlyingObject(){

}
void UninitFlyingObject(){

}
void DrawFlyingObject(){

}
void UpdateFlyingObject(){

}