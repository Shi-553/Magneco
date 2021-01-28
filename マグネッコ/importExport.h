#pragma once
#include <string>
#include <vector>
#include "stageInfo.h"


void SetStagePath(std::string filename);
bool StageExport();
StageInfo* StageImport();
bool GetStageInfos(std::string foldername, std::vector<StageInfo>& infos);