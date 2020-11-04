#include "flyingObjectSponer.h"
#include "flyingObject.h"
#include "map.h"

#define SPONE_MAX 100

typedef struct Spone {
	int frame;
	FlyingObject flyingObject;
};

static Spone spones[SPONE_MAX]{
	{100,{{5,-3},FLYING_OBJECT_BLOCK,{0,1}}},
	{150,{{-3,8},FLYING_OBJECT_BLOCK,{1,0}}},
	{200,{{13,5},FLYING_OBJECT_ENEMY,{-1,0}}},
	{250,{{13,1},FLYING_OBJECT_BLOCK,{-1,0}}},
	{300,{{8,13},FLYING_OBJECT_BLOCK,{0,-1}}},
	{350,{{3,-3},FLYING_OBJECT_ENEMY,{0,1}}},
	{400,{{2,13},FLYING_OBJECT_BLOCK,{0,-1}}},
	{450,{{13,3},FLYING_OBJECT_BLOCK,{-1,0}}},
	{500,{{5,13},FLYING_OBJECT_BLOCK,{0,-1}}},
	{550,{{-3,6},FLYING_OBJECT_ENEMY,{0,1}}},
	{600,{{3,13},FLYING_OBJECT_BLOCK,{0,-1}}},
	{650,{{13,7},FLYING_OBJECT_BLOCK,{-1,0}}},
	{700,{{-3,5},FLYING_OBJECT_BLOCK,{1,0}}},
	{750,{{-3,8},FLYING_OBJECT_BLOCK,{1,0}}},
	{800,{{3,13},FLYING_OBJECT_ENEMY,{0,-1}}},
	{850,{{13,5},FLYING_OBJECT_BLOCK,{-1,0}}},
	{900,{{2,13},FLYING_OBJECT_BLOCK,{0,-1}}},
	{950,{{8,-3},FLYING_OBJECT_ENEMY,{0,1}}},
	{1000,{{-3,8},FLYING_OBJECT_BLOCK,{1,0}}},
	{1050,{{13,3},FLYING_OBJECT_BLOCK,{-1,0}}},
	{1100,{{6,13},FLYING_OBJECT_BLOCK,{0,-1}}},
	{1150,{{4,-3},FLYING_OBJECT_ENEMY,{0,1}}},
	{1200,{{13,1},FLYING_OBJECT_BLOCK,{-1,0}}},
	{1250,{{7,13},FLYING_OBJECT_BLOCK,{0,-1}}},
	{-1,  {{-3,2},FLYING_OBJECT_BLOCK,{0,0}}}
};
static int frame = 0;
static int sponeIndex = 0;

void InitFlyingSponer() {
}
void UninitFlyingSponer() {

}
void UpdateFlyingSponer() {
	while (spones[sponeIndex].frame != -1) {
		if (spones[sponeIndex].frame <= frame) {
			GetFlyingObjects()->push_back(spones[sponeIndex].flyingObject);
			sponeIndex++;
		}
		else {
			break;
		}
	}
	frame++;
}

