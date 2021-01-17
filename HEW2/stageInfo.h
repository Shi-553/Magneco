#pragma once
#include <stdio.h>

struct StageInfo
{
	int index;
	int level;
	char* name;
	char* overview;
};
void InitStageInfo();
void UninitStageInfo();
bool ImportStageInfo(FILE* fp);
StageInfo& ExportStageInfo(FILE* fp);
StageInfo& GetStageInfo();