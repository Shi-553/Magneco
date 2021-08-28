#pragma once
#include <string>
#include <vector>
#include "stageInfo.h"


void SetStagePath(std::string filename);
const std::string& GetStagePath( );
bool StageExport();
StageInfo* StageImport();
bool GetStageInfos(std::string foldername, std::vector<StageInfo>& infos);