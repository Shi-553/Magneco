#include "stageInfo.h"
#include <vector>

using namespace std;

static StageInfo info;


void StageToSamune(std::string& stagename);

void InitStageInfo() {
	UninitStageInfo();
	info.index = 0;
	info.level = 1;
	info.name = "asd";
	info.overview = "ああああ\nああ\nあああああああああああああああ";
}
void UninitStageInfo() {
}


StageInfo ImportStageInfo(FILE* fp,string& filename) {
	UninitStageInfo();

	fread(&info.index, sizeof(int), 1, fp);
	fread(&info.level, sizeof(int), 1, fp);

	int nameLen,overviewLen;
	fread(&nameLen, sizeof(int), 1, fp);

	auto name = new char[nameLen];
	fread(name, sizeof(char), nameLen, fp);
	info.name = name;
	delete[] name;

	fread(&overviewLen, sizeof(int), 1, fp);
	auto overview = new char[overviewLen];
	fread(overview, sizeof(char), overviewLen, fp);
	//fread(&info.overview, sizeof(char), overviewLen, fp);
	info.overview = overview;
	delete[] overview;

	info.filename = filename;

	info.samunename = filename;
	StageToSamune(info.samunename);

	return info;
}

bool ExportStageInfo(FILE* fp) {

	//	ファイルへの書き込み処理
	fwrite(&info.index, sizeof(int), 1, fp);
	fwrite(&info.level, sizeof(int), 1, fp);

	int nameLen=info.name.size()+1, overviewLen= info.overview.size() + 1;

	fwrite(&nameLen, sizeof(int), 1, fp);

	fwrite(info.name.c_str(), sizeof(char), nameLen, fp);
	fwrite(&overviewLen, sizeof(int), 1, fp);

	fwrite(info.overview.c_str(), sizeof(char), overviewLen, fp);


	return true;

}

StageInfo& GetStageInfo() {
	return info;
}

void StageToSamune(std::string& stagename) {
	stagename.replace(stagename.size() - 6, 6, ".png");
}