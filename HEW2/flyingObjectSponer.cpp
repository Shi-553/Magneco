#include "flyingObjectSponer.h"
#include "flyingObject.h"

#define SPONE_MAX 100

typedef struct Spone {
	int frame;
	FlyingObject flyingObject;
};

static Spone spones[SPONE_MAX]{
	{100, FlyingObject({5,-3},FLYING_OBJECT_BLOCK,{0,1}) },
	{500, FlyingObject({-3,13},FLYING_OBJECT_BLOCK,{1,0}) },
	{1000,FlyingObject({-3,9},FLYING_OBJECT_BLOCK,{1,0}) },
	{1100,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{1200,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{2000,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{2400,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{3000,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{3300,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{3300,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{3500,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{4000,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
	{-1,FlyingObject({-3,0},FLYING_OBJECT_BLOCK,{0,1}) },
};
static int frame = 0;
static int sponeIndex = 0;

void InitFlyingSponer() {

}
void UninitFlyingSponer() {

}
void UpdateFlyingSponer() {
	while (spones[sponeIndex].frame != -1 ) {
		if (spones[sponeIndex].frame >= frame) {
			GetFlyingObjects()->push_back(spones[sponeIndex].flyingObject);
			sponeIndex++;
		}
	}
	frame++;
}

