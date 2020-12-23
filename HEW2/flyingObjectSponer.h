#pragma once
#include <stdio.h>
#include "myd3d.h"
#include "flyingObject.h"
struct Spone {
	int frame;
	D3DXVECTOR2 initPos;
	FlyingObjectType type;
	D3DXVECTOR2 dir;
};

void InitFlyingSponer();
void UninitFlyingSponer();
void UpdateFlyingSponer();

void SetFlyingObjectSponerLoop(bool f);
bool GetFlyingObjectSponerLoop();

bool FlyingObjectSponerExport(FILE* fp);
bool FlyingObjectSponerImport(FILE* fp);

int GetFlyingObjectSponeFrame();
void SetFlyingObjectSponeFrame(int f);