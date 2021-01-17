﻿#pragma once
#include <stdio.h>
#include <string>
#include "texture.h"


struct StageInfo
{
	int index;
	int level;
	std::string name;
	std::string overview;
	std::string filename;
	std::string samunename;
};

void InitStageInfo();
void UninitStageInfo();
StageInfo& ImportStageInfo(FILE* fp, std::string& filename);
 bool ExportStageInfo(FILE* fp);
StageInfo& GetStageInfo();