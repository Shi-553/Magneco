#include "flyingObjectSponer.h"
#include "flyingObject.h"

#define SPONE_MAX 100

typedef struct Spone {
	int frame;
	FlyingObject flyingObject;
};

static Spone spones[SPONE_MAX]{
	{100, {{5,-3},FLYING_OBJECT_BLOCK,{0,1}}},
	{200, {{-3,13},FLYING_OBJECT_BLOCK,{1,0}} },
	{300,{{-3,9},FLYING_OBJECT_ENEMY,{1,0} }},
	{400,{{-3,5},FLYING_OBJECT_BLOCK,{1,0} }},
	{500,{{-3,6},FLYING_OBJECT_BLOCK,{1,0} }},
	{600,{{-3,2},FLYING_OBJECT_ENEMY,{1,0} }},
	{700,{{-3,8},FLYING_OBJECT_BLOCK,{1,0} }},
	{800,{{-3,3},FLYING_OBJECT_BLOCK,{1,0} }},
	{900,{{-3,5},FLYING_OBJECT_BLOCK,{1,0} }},
	{1000,{{-3,8},FLYING_OBJECT_ENEMY,{1,0} }},
	{1100,{{-3,0},FLYING_OBJECT_BLOCK,{1,0} }},
	{1200,{{-3,7},FLYING_OBJECT_BLOCK,{1,0} }},
	{-1,  {{-3,2},FLYING_OBJECT_BLOCK,{0,0} }}
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

