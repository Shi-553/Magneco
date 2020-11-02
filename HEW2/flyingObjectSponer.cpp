#include "flyingObjectSponer.h"
#include "flyingObject.h"

#define SPONE_MAX 100

typedef struct Spone {
	int frame;
	FlyingObject flyingObject;
};

static Spone spones[SPONE_MAX]{
	{100, {{5,-3},FLYING_OBJECT_BLOCK,{0,1}}},
	{500, {{-3,13},FLYING_OBJECT_BLOCK,{1,0}} },
	{1000,{{-3,9},FLYING_OBJECT_BLOCK,{1,0} }},
	{1100,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{1200,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{2000,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{2400,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{3000,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{3300,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{3300,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{3500,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{4000,{{-3,0},FLYING_OBJECT_BLOCK,{0,1} }},
	{-1,  {{-3,0},FLYING_OBJECT_BLOCK,{0,1} }}
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

