﻿
#include "config.h"
#include "stageInfoEditor.h"
#include "messeage.h"
#include "stageInfo.h"
#include <regex>
#include "InputLogger.h"
#include "stageEditor.h"
#include <vector>
#include<iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "importExport.h"
#include "StringConvert.hpp"

static Message* infoMessage;
static StageInfo& info = GetStageInfo();

std::string path = "";


OPENFILENAME CreateOPENFILENAME(const LPSTR& path, const std::string& initPath);



OPENFILENAME CreateOPENFILENAME(const LPSTR& path, const std::string& initPath) {

	OPENFILENAME open{};
	open.lpstrFilter = "ステージファイル(*.stage)\0*.stage\0全てのファイル(*.*)\0*.*\0";
	open.lStructSize = sizeof(OPENFILENAME);
	//open.hwndOwner = GetWindow();
	open.lpstrFile = path;
	open.nMaxFile = MAX_PATH;
	open.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
	open.lpstrDefExt = "stage";

	open.lpstrInitialDir = initPath.c_str();

	return open;
}

void InitStageInfoEditor() {
	infoMessage = new Message(D3DXVECTOR2(0.8, 0.8));
}
void UninitStageInfoEditor() {
	delete infoMessage;
}
void DrawStageInfoEditor() {
	auto oneSize = infoMessage->GetOneSize();

	infoMessage->ClearOffset();
	infoMessage->SetPos(D3DXVECTOR2(10, SCREEN_HEIGHT - oneSize.y));
	infoMessage->Draw(info.filename.c_str());


	ImGui::SetNextWindowPos(ImVec2(300, 500), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 230), ImGuiCond_Once);
	if (ImGui::Begin("StageSetting",nullptr, ImGuiWindowFlags_MenuBar)) {//, &isEdit]

		ImGui::PushID(&info.name);

		std::string nameBuf = StringConvert::MultiToUtf8(info.name);
		nameBuf.resize(256);


		ImGui::Text("Name:     ");
		ImGui::SameLine();
		if (ImGui::InputText("", (char*)nameBuf.c_str(), nameBuf.capacity() + 1)) {
			info.name = StringConvert::Utf8ToMulti(nameBuf);
		}
		ImGui::PopID();

		ImGui::PushID(&info.overview);

		std::string overviewBuf = StringConvert::MultiToUtf8(info.overview);
		overviewBuf.resize(256);

		ImGui::Text("Overview: ");
		ImGui::SameLine();
		if (ImGui::InputTextMultiline("", (char*)overviewBuf.c_str(), overviewBuf.capacity() + 1, ImVec2(120, 50))) {
			info.overview = StringConvert::Utf8ToMulti(overviewBuf);
		}
		ImGui::PopID();

		ImGui::PushID(&info.level);
		ImGui::Text("Level: ");
		ImGui::SameLine();
		if (ImGui::InputInt("", &info.level)) {
			info.level = max(info.level, 0);
		}
		ImGui::PopID();

		ImGui::PushID(&info.index);
		ImGui::Text("Index: ");
		ImGui::SameLine();
		if (ImGui::InputInt("", &info.index)) {
			info.index = max(info.index, 0);
		}
		ImGui::PopID();

		ImGui::PushID(&info.sLabel);
		ImGui::Text("BGM: ");
		ImGui::SameLine();
		if (ImGui::SliderInt("", (int*)&info.sLabel,0, SOUND_LABEL_BGM006)) {

		}
		ImGui::PopID();


		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) {
					if (path == "") {
						char strFile[MAX_PATH] = "";
						auto open = CreateOPENFILENAME(strFile, path);

						if (GetSaveFileName(&open)) {
							path = strFile;
						}
					}
					if (path != "") {
						SetStagePath(path);
						StageExport();
					}
				}
				if (ImGui::MenuItem("Save as...")) {
					char strFile[MAX_PATH] = "";
					auto open = CreateOPENFILENAME(strFile, path);

					if (GetSaveFileName(&open)) {
						path = strFile;
					}
					if (path != "") {
						SetStagePath(path);
						StageExport();
					}

				}
				if (ImGui::MenuItem("Load")) {
					char strFile[MAX_PATH] = "";
					auto open = CreateOPENFILENAME(strFile, path);

					if (GetOpenFileName(&open)) {
						path = strFile;
					}
					if (path != "") {
						SetStagePath(path);
						StageImport();
					}
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
	ImGui::End();
}
void UpdateStageInfoEditor() {

	if (PressInputLogger(MYVK_CONTROL) && TriggerInputLogger(MYVK_SAVE)) {
		StageExport();

	}
	if (PressInputLogger(MYVK_CONTROL) && TriggerInputLogger(MYVK_LOAD)) {
		StageImport();
	}
}
bool CheckMouseStageInfoEditor() {

	return false;
}