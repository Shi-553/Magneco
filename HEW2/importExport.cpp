#include <stdio.h>
#include "map.h"
#include "flyingObjectSponer.h"
#include "npc.h"
#include "player.h"

bool StageExport(const char* filename) {
	FILE* fp = NULL;

	//	バイナリ書き込みモードでファイルを開く
	fopen_s(&fp, filename, "wb");

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
bool StageImport(const char* filename) {
	FILE* fp = NULL;

	//	バイナリ読み込みモードでファイルを開く
	fopen_s(&fp, filename, "rb");

	if (fp == NULL) {
		return false;
	}

	PlayerImport(fp);
	NPCImport(fp);
	MapImport(fp);
	FlyingObjectSponerImport(fp);

	fclose(fp);
	return true;
}