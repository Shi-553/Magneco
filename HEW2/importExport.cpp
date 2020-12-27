﻿#include <stdio.h>
#include "map.h"
#include "flyingObjectSponer.h"
#include "npc.h"
#include "player.h"
#include "importExport.h"

static const char* gFilename;
void SetStagePath(const char* filename) {
	gFilename = filename;
}
bool StageExport() {
	FILE* fp = NULL;

	//	バイナリ書き込みモードでファイルを開く
	fopen_s(&fp, gFilename, "wb");

	if (fp == NULL) {
		return false;
	}

	PlayerExport(fp);
	NPCExport(fp);
	MapExport(fp);
	FlyingObjectSponerExport(fp);

	fclose(fp);

	return true;
}
bool StageImport() {
	FILE* fp = NULL;

	//	バイナリ読み込みモードでファイルを開く
	fopen_s(&fp, gFilename, "rb");

	if (fp == NULL) {
		return false;
	}

	PlayerImport(fp);
	NPCImport(fp);
	MapImport(fp);
	SecureMapLabelList();
	FlyingObjectSponerImport(fp);

	fclose(fp);
	return true;
}