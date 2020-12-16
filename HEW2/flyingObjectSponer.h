#pragma once
#include <stdio.h>

void InitFlyingSponer();
void UninitFlyingSponer();
void UpdateFlyingSponer();

bool FlyingObjectSponerExport(FILE* fp);
bool FlyingObjectSponerImport(FILE* fp);