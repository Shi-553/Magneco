#include "stageInfoEditor.h"
#include "messeage.h"
#include "stageInfo.h"
#include "config.h"
#include <regex>
#include "InputLogger.h"
#include "stageEditor.h"
#include <vector>
#include<iostream>

static Message* infoMessage;
static StageInfo& info = GetStageInfo();

void Paste(std::string& str);

#define POS_Y 60

#define NAME_POS D3DXVECTOR2(10, POS_Y)
#define FILENAME_POS D3DXVECTOR2(10, SCREEN_HEIGHT - oneSize.y)
#define OVERVIEW_POS D3DXVECTOR2(10, POS_Y + oneSize.y)
#define LEVEL_POS D3DXVECTOR2(10, POS_Y + infoMessage->GetOneSize().y*2)
#define INDEX_POS D3DXVECTOR2(10, POS_Y + infoMessage->GetOneSize().y*3)


std::string name;
std::string overview;
std::string level;
std::string index;

void InitStageInfoEditor() {
	infoMessage = new Message(D3DXVECTOR2(0.8, 0.8));
}
void UninitStageInfoEditor() {
	delete infoMessage;
}
void DrawStageInfoEditor() {
	auto oneSize = infoMessage->GetOneSize();

	name = "NAME:" + info.name;

	overview = "OVERVIEW:" + std::regex_replace(info.overview, std::regex("\n"), "/n");

	level = "LEVEL:" + std::to_string(info.level);
	 index = "INDEX:" + std::to_string(info.index);

	infoMessage->ClearOffset();
	infoMessage->SetPos(FILENAME_POS);
	infoMessage->Draw(info.filename.c_str());

	infoMessage->ClearOffset();
	infoMessage->SetPos(NAME_POS);
	infoMessage->Draw(name.c_str());

	infoMessage->ClearOffset();
	infoMessage->SetPos(OVERVIEW_POS);
	infoMessage->Draw(overview.c_str());

	infoMessage->ClearOffset();
	infoMessage->SetPos(LEVEL_POS);
	infoMessage->Draw(level.c_str());

	infoMessage->ClearOffset();
	infoMessage->SetPos(INDEX_POS);
	infoMessage->Draw(index.c_str());
}
void UpdateStageInfoEditor() {

}
bool CheckMouseStageInfoEditor() {
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));

	auto oneSize = infoMessage->GetOneSize();

	if (PressInputLogger(MYVK_CONTROL) && ReleaseInputLogger(MYVK_PASTE)) {
		if (CheckSquare(mousePos, NAME_POS, D3DXVECTOR2(name.size() * oneSize.x, oneSize.y))) {
			Paste(info.name);
		}
		if (CheckSquare(mousePos, OVERVIEW_POS, D3DXVECTOR2(overview.size() * oneSize.x, oneSize.y))) {
			Paste(info.overview);
		}
	}

	auto wheel = GetInputLoggerAxisAmountInt(MYVA_MW);
	if (wheel != 0) {
		if (CheckSquare(mousePos, LEVEL_POS, D3DXVECTOR2(level.size() * oneSize.x, oneSize.y))) {
			info.level += wheel/120;
		}
		if (CheckSquare(mousePos, INDEX_POS, D3DXVECTOR2(index.size() * oneSize.x, oneSize.y))) {
			info.index += wheel/120;
		}
	}
	return false;
}

void Paste(std::string& str) {

	HANDLE hg;
	if (OpenClipboard(GetActiveWindow()) && (hg = GetClipboardData(CF_TEXT))) {
		if (auto strClip = GlobalLock(hg)) {
			str = (LPSTR)strClip;
			GlobalUnlock(hg);
		}
		CloseClipboard();
	}
}