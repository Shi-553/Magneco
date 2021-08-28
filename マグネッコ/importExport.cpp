﻿#include <stdio.h>
#include "map.h"
#include "flyingObjectSponer.h"
#include "npc.h"
#include "player.h"
#include "importExport.h"


std::string gFilename="";

void SetStagePath(std::string filename) {
	gFilename = filename;
}
const std::string& GetStagePath(){
	return gFilename;
}
bool StageExport() {
	FILE* fp = NULL;

	//	バイナリ書き込みモードでファイルを開く
	fopen_s(&fp, gFilename.c_str(), "wb");

	if (fp == NULL) {
		return false;
	}

	ExportStageInfo(fp);

	MapExport(fp);
	FlyingObjectSponerExport(fp);
	PlayerExport(fp);
	NPCExport(fp);

	fclose(fp);

	return true;
}
StageInfo* StageImport() {
	FILE* fp = NULL;

	//	バイナリ読み込みモードでファイルを開く
	fopen_s(&fp, gFilename.c_str(), "rb");

	if (fp == NULL) {
		return nullptr;
	}

	auto info=ImportStageInfo(fp, gFilename);

	MapImport(fp);
	FlyingObjectSponerImport(fp);
	PlayerImport(fp);
	NPCImport(fp);

	SecureMapLabelList();

	fclose(fp);
	return info;
}


bool GetStageInfos(std::string foldername, std::vector<StageInfo>& infos) {
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	auto search_name = foldername + "\\*";

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}

	/* 指定のディレクトリ以下のファイル名をファイルがなくなるまで取得する */
	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			/* ディレクトリの場合は何もしない */
			printf("directory\n");
		}
		else {
			std::string filename = std::string(win32fd.cFileName);
			if (filename.rfind(".stage") != filename.size() - 6) {
				continue;
			}

			FILE* fp = NULL;
			std::string filePath=foldername +"\\" + filename;

			fopen_s(&fp, filePath.c_str(), "rb");

			if (fp != NULL) {
				StageInfo f= *ImportStageInfo(fp, filePath);
				
				infos.push_back(f);

				fclose(fp);
			}
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return true;
}