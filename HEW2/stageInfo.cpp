#include "stageInfo.h"
#include <vector>
#include <string>
#include "texture.h"

using namespace std;

static StageInfo info;

static int samuneTextureId = TEXTURE_INVALID_ID;

void InitStageInfo() {
	UninitStageInfo();
	info.index = 0;
	info.level = 0;
	info.name = new char[] {""};
	info.overview = new char[] {""};
}
void UninitStageInfo() {
	if (info.name != NULL) {
		delete[] info.name;
		info.name = NULL;
	}
	if (info.overview != NULL) {
		delete[] info.overview;
		info.overview = NULL;
	}
	ReleaseTexture(samuneTextureId);
}


StageInfo& ImportStageInfo(FILE* fp,string& filename) {
	UninitStageInfo();

	fread(&info.index, sizeof(int), 1, fp);
	fread(&info.level, sizeof(int), 1, fp);

	int nameLen,overviewLen;
	fread(&nameLen, sizeof(int), 1, fp);

	info.name = new char[nameLen];
	fread(info.name, sizeof(char), nameLen, fp);

	fread(&overviewLen, sizeof(int), 1, fp);
	info.overview = new char[overviewLen];
	fread(info.overview, sizeof(char), overviewLen, fp);

	samuneTextureId = ReserveTextureLoadFile(filename.c_str());
	return info;
}

bool ExportStageInfo(FILE* fp) {

	//	ファイルへの書き込み処理
	fwrite(&info.index, sizeof(int), 1, fp);
	fwrite(&info.level, sizeof(int), 1, fp);

	int nameLen=strlen(info.name)+1, overviewLen= strlen(info.overview) + 1;

	fwrite(&nameLen, sizeof(int), 1, fp);

	fwrite(info.name, sizeof(char), nameLen, fp);
	fwrite(&overviewLen, sizeof(int), 1, fp);

	fwrite(info.overview, sizeof(char), overviewLen, fp);


	return true;

}

StageInfo& GetStageInfo() {
	return info;
}