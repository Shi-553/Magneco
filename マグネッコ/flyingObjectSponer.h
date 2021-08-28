﻿#pragma once
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
	float speed;
};
struct SponeId {
	Spone s;
	int id;
	int isAlive;
};

void InitFlyingSponer();
void UninitFlyingSponer();
void UpdateFlyingSponer();
void DrawFlyingSponer();

void SetFlyingObjectSponerLoop(bool f);
bool GetFlyingObjectSponerLoop();

void AddFlyingObjectSponer(Spone s);
void RemoveFlyingObjectSponer(int id);

bool FlyingObjectSponerExport(FILE* fp);
bool FlyingObjectSponerImport(FILE* fp);

int GetFlyingObjectSponeFrameMax();
int GetFlyingObjectSponeFrame();
void SetFlyingObjectSponeFrame(int f);

void DestrySpone(FlyingObject& f);