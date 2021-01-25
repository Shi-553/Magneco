#pragma once
#include "stageInfo.h"


void InitStageSelect();
void UninitStageSelect();
void UpdateStageSelect();
void DrawStageSelect();

void SetStageFolder(std::string foldername);

StageInfo* GetCurrentInfo();