#pragma once
#include <stdio.h>
#include "myd3d.h"
#include "flyingObject.h"
struct Spone {
	int frame;
	D3DXVECTOR2 initPos;
	FlyingObjectType type;
	D3DXVECTOR2 dir;
	int hp;
	INTVECTOR2 size;
};
struct SponeId {
	Spone s;
	int id;
};

void InitFlyingSponer();
void UninitFlyingSponer();
void UpdateFlyingSponer();

void SetFlyingObjectSponerLoop(bool f);
bool GetFlyingObjectSponerLoop();

void AddFlyingObjectSponer(Spone s);
void RemoveFlyingObjectSponer(int id);

bool FlyingObjectSponerExport(FILE* fp);
bool FlyingObjectSponerImport(FILE* fp);

int GetFlyingObjectSponeFrame();
void SetFlyingObjectSponeFrame(int f);