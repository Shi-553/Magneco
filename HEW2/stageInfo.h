#pragma once
#include <stdio.h>
#include <string>

struct StageInfo
{
	int index;
	int level;
	char* name;
	char* overview;
};

void InitStageInfo();
void UninitStageInfo();
StageInfo& ImportStageInfo(FILE* fp, std::string& filename);
 bool ExportStageInfo(FILE* fp);
StageInfo& GetStageInfo();