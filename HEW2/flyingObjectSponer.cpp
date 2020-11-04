#include "flyingObjectSponer.h"
#include "flyingObject.h"

#define SPONE_MAX 100

typedef struct Spone {
	int frame;
	FlyingObject flyingObject;
};

static Spone spones[SPONE_MAX]{
	{100, {{5,-3},FLYING_OBJECT_BLOCK,{0,1}}},
	{105, {{-3,13},FLYING_OBJECT_BLOCK,{1,0}} },
	{110,{{-3,9},FLYING_OBJECT_ENEMY,{1,0} }},
	{115,{{-3,5},FLYING_OBJECT_BLOCK,{1,0} }},
	{120,{{-3,6},FLYING_OBJECT_BLOCK,{1,0} }},
	{125,{{-3,2},FLYING_OBJECT_ENEMY,{1,0} }},
	{130,{{-3,8},FLYING_OBJECT_BLOCK,{1,0} }},
	{135,{{-3,3},FLYING_OBJECT_BLOCK,{1,0} }},
	{140,{{-3,5},FLYING_OBJECT_BLOCK,{1,0} }},
	{145,{{-3,8},FLYING_OBJECT_ENEMY,{1,0} }},
	{150,{{-3,0},FLYING_OBJECT_BLOCK,{1,0} }},
	{155,{{-3,7},FLYING_OBJECT_BLOCK,{1,0} }},
	{160, {{5,-3},FLYING_OBJECT_BLOCK,{0,1}}},
	{165, {{-3,13},FLYING_OBJECT_BLOCK,{1,0}} },
	{170,{{-3,9},FLYING_OBJECT_ENEMY,{1,0} }},
	{175,{{-3,5},FLYING_OBJECT_BLOCK,{1,0} }},
	{180,{{-3,6},FLYING_OBJECT_BLOCK,{1,0} }},
	{185,{{-3,2},FLYING_OBJECT_ENEMY,{1,0} }},
	{190,{{-3,8},FLYING_OBJECT_BLOCK,{1,0} }},
	{195,{{-3,3},FLYING_OBJECT_BLOCK,{1,0} }},
	{200,{{-3,5},FLYING_OBJECT_BLOCK,{1,0} }},
	{205,{{-3,8},FLYING_OBJECT_ENEMY,{1,0} }},
	{210,{{-3,0},FLYING_OBJECT_BLOCK,{1,0} }},
	{215,{{-3,7},FLYING_OBJECT_BLOCK,{1,0} }},
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

