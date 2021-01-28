#pragma once
#include "stageInfo.h"


void InitStageSelect();
void UninitStageSelect();
void UpdateStageSelect();
void DrawStageSelect();
bool IsNextStage();
void NextStaege();

void SetStageFolder(std::string foldername);

StageInfo* GetCurrentInfo();

const std::string& GetStageFoldername();