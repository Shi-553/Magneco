#pragma once
#include <string>
#include <vector>
#include "stageInfo.h"


void SetStagePath(std::string filename);
bool StageExport();
bool StageImport();
bool GetStageInfos(std::string foldername, std::vector<StageInfo>& infos);