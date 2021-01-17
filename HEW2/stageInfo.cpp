#include "stageInfo.h"
#include <vector>
using namespace std;

StageInfo info;

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
}


bool ImportStageInfo(FILE* fp) {
	UninitStageInfo();

	fread(&info.index, sizeof(int), 1, fp);
	fread(&info.level, sizeof(int), 1, fp);

	int nameLen,overviewLen;
	fread(&nameLen, sizeof(int), 1, fp);

	info.name = new char[nameLen];
	fread(&info.name, sizeof(char), nameLen, fp);

	fread(&overviewLen, sizeof(int), 1, fp);
	info.overview = new char[overviewLen];
	fread(&info.overview, sizeof(char), overviewLen, fp);

	return true;
}

StageInfo& ExportStageInfo(FILE* fp) {

	//	ファイルへの書き込み処理
	fwrite(&info.index, sizeof(int), 1, fp);
	fwrite(&info.level, sizeof(int), 1, fp);

	int nameLen=sizeof(info.name)+1, overviewLen=sizeof(info.overview) + 1;

	fwrite(&nameLen, sizeof(int), 1, fp);

	fwrite(&info.name, sizeof(char), nameLen, fp);
	fwrite(&overviewLen, sizeof(int), 1, fp);

	fwrite(&info.overview, sizeof(char), overviewLen, fp);


	return info;

}

StageInfo& GetStageInfo() {
	return info;
}