#include "flyingObjectEditor.h"
#include "InputLogger.h"
#include "debugPrintf.h"
#include "flyingObjectSponer.h"
#include "flyingObject.h"
#include "debugFont.h"
#include "texture.h"
#include "sprite.h"
#include "stageEditor.h"
#include "gameSrite.h"
#include "npc.h"
#include "imgui/imgui.h"
#include <string>
#include "messeage.h"
#include "pauseMenu.h"

#define CREATE_FLYING_OBJECT_TEXTURE_WIDTH 32
#define CREATE_FLYING_OBJECT_TEXTURE_HEIGHT 32


#define CREATE_FLYING_OBJECT_ZERO_X 1050
#define CREATE_FLYING_OBJECT_ZERO_Y 250

#define CREATE_FLYING_OBJECT_WIDTH 50
#define CREATE_FLYING_OBJECT_HEIGHT 50

#define CREATE_FLYING_OBJECT_MARGIN 20

#define CREATE_PROPERTY_X 1000





D3DXVECTOR2 GetCreateFlyingObjectPos(int type);
void DrawFlyingObjectScreen(const Spone& map);
Spone CheckCreateFlyingObject(D3DXVECTOR2 pos);
void SetSpeed(float speed);

static bool isPlay = false;
static bool isDrag = false;

static Spone creates[FLYING_OBJECT_MAX];
static int textureIds[FLYING_OBJECT_MAX];

static int eraseTextureId = TEXTURE_INVALID_ID;
static int flyingObjectCreateFrame = TEXTURE_INVALID_ID;
static bool isErase = false;

static Spone current;
static D3DXVECTOR2 startDrag;

static int createHP = 1;
static INTVECTOR2 createSize = { 1,1 };
static float createSpeed = 1.0f;

Message* flyingObjectEditorMessage;

void SetSpeed(float speed) {
	createSpeed = speed;
	createSpeed *= 10.0;
	createSpeed = round(createSpeed);
	createSpeed /= 10.0;

	if (createSpeed < 0) {
		createSpeed = 0;
	}
}

void InitFlyingObjectEditor() {
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++)
	{
		textureIds[i] = GetFlyingObjectTextureId((FlyingObjectType)i);
	}


	eraseTextureId = ReserveTextureLoadFile("texture/block/MAP_BLOCK_NONE_ERASE2.png");
	flyingObjectCreateFrame = ReserveTextureLoadFile("texture/ui/flyingObjectCreateFrame.png");


	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		creates[i].type = (FlyingObjectType)i;
		creates[i].initPos = GetCreateFlyingObjectPos(i+2);
		creates[i].size = { 1,1 };
	}

	flyingObjectEditorMessage = new Message({ 1.5f,1.5f });

	SetFlyingObjectSponerLoop(false);
}
void UninitFlyingObjectEditor() {
	ReleaseTexture(eraseTextureId);
	ReleaseTexture(flyingObjectCreateFrame);

	delete flyingObjectEditorMessage;
}

void DrawFlyingObjectEditor() {
	DrawFlyingObject();

	flyingObjectEditorMessage->SetFormat(DT_CENTER | DT_NOCLIP);

	flyingObjectEditorMessage->SetPosEndPos({ SCREEN_WIDTH / 2,0 });
	flyingObjectEditorMessage->ClearOffset();
	flyingObjectEditorMessage->Draw("%d", GetFlyingObjectSponeFrame());

	ImGui::SetNextWindowPos(ImVec2(CREATE_PROPERTY_X, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 130), ImGuiCond_Once);

	if (!IsPause() &&ImGui::Begin("CreateObjectSetting")) {


		ImGui::PushID(&createHP);
		ImGui::Text("HP: ");
		ImGui::SameLine();
		if (ImGui::InputInt("", &createHP)) {
			createHP = max(createHP, 0);
		}
		ImGui::PopID();

		ImGui::PushID(&createSize);
		ImGui::Text("Size: ");
		ImGui::SameLine();
		if (ImGui::InputInt2("", (int*)&createSize)) {
			createSize.x = max(createSize.x, 0);
			createSize.y = max(createSize.y, 0);
		}
		ImGui::PopID();

		ImGui::PushID(&createSpeed);
		ImGui::Text("Speed: ");
		ImGui::SameLine();
		if (ImGui::InputFloat("",&createSpeed)) {
			createSpeed = max(createSpeed, 0);
		}
		ImGui::PopID();

		ImGui::End();
	}




	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		DrawFlyingObjectScreen(creates[i]);
	}


	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));

	D3DXVECTOR2 size = D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT);

	D3DXVECTOR2 tPos = { 0,0 };
	D3DXVECTOR2 tSize = { CREATE_FLYING_OBJECT_TEXTURE_WIDTH,CREATE_FLYING_OBJECT_TEXTURE_HEIGHT };

	DrawSprite(eraseTextureId, GetCreateFlyingObjectPos(-1), 10, size, tPos, tSize);

	if (isErase) {
		DrawSprite(eraseTextureId, mousePos - D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT) / 2, 10, size, tPos, tSize);
	}
	else
		if (current.type != FLYING_OBJECT_NONE) {
			Spone s = current;
			s.initPos = GameToScreenPos(s.initPos) - D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT) / 2;
			DrawFlyingObjectScreen(s);
		}

	DrawSprite(flyingObjectCreateFrame, GetCreateFlyingObjectPos(-1), 10);


	ImGui::SetNextWindowPos(ImVec2(500, 500), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Once);

	if (!IsPause() && ImGui::Begin("PlayControl")) {
		if (ImGui::Button("Start/Pause")) {
			isPlay = !isPlay;
		}
		if (ImGui::Button("Restart")) {
			isPlay = true;
			GetFlyingObjects()->clear();
			FlyingObjectDeleteUFO();
			SetFlyingObjectSponeFrame(0);
		}
		bool l = GetFlyingObjectSponerLoop();
		if (ImGui::Checkbox("Loop", &l)) {
			SetFlyingObjectSponerLoop(l);
			GetFlyingObjects()->clear();
			FlyingObjectDeleteUFO();
			SetFlyingObjectSponeFrame(0);
		}
		auto s = "MAX: " + std::to_string(GetFlyingObjectSponeFrameMax());
		ImGui::Text(s.c_str());

		ImGui::End();
	}

}
void UpdateFlyingObjectEditor() {
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));




	auto wheel = GetInputLoggerAxisAmountInt(MYVA_MW);
	//DebugPrintf("%d,\n", wheel);

	if (wheel != 0) {
		auto base = PressInputLogger(MYVK_SHIFT) ? 5 : 1;

			isPlay = false;
			SetFlyingObjectSponeFrame(GetFlyingObjectSponeFrame() + (wheel / 10) * base);
	}

	if (TriggerInputLogger(MYVK_L)) {
		SetFlyingObjectSponerLoop(!GetFlyingObjectSponerLoop());
		GetFlyingObjects()->clear();
		FlyingObjectDeleteUFO();
		SetFlyingObjectSponeFrame(0);
	}
	if (TriggerInputLogger(MYVK_START_STOP)) {
		isPlay = !isPlay;
	}

	if (TriggerInputLogger(MYVK_FIRST_START)) {
		isPlay = true;
		GetFlyingObjects()->clear();
		FlyingObjectDeleteUFO();
		SetFlyingObjectSponeFrame(0);
	}

	if (isPlay) {
		UpdateFlyingSponer();
		UpdateFlyingObject();
	}


}
bool CheckMouseFlyingObjectEditor() {
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));
	bool ret = false;
	if (isErase) {
		ret = true;
	}

	if (TriggerInputLogger(MYVK_LEFT_CLICK)) {
		startDrag = mousePos;
	}
	if (current.type != FLYING_OBJECT_NONE && !isDrag) {
		current.initPos = ScreenToGamePos(mousePos);
	}
	if (PressInputLogger(MYVK_LEFT_CLICK)) {
		if (!isDrag) {
			auto diff = mousePos - startDrag;
			float diffLength = D3DXVec2Length(&diff);
			if (diffLength > 10) {
				isDrag = true;
			}
		}
		if (isErase && isDrag) {
			auto flyingObjectList = GetFlyingObjects();
			for (auto itr = flyingObjectList->begin(); itr != flyingObjectList->end(); ) {
				if (CheckSquare(mousePos, GameToScreenPos(itr->trans.pos) - D3DXVECTOR2(25, 25), { 50,50 })) {
					RemoveFlyingObjectSponer(itr->id);
					itr = flyingObjectList->erase(itr);
				}
				else {
					itr++;
				}
			}
		}
	}
	if (ReleaseInputLogger(MYVK_LEFT_CLICK)) {

		if (!isDrag) {
			isErase = false;

		}
		isDrag = false;
		if (current.type == FLYING_OBJECT_NONE) {
			current = CheckCreateFlyingObject(mousePos);
		}
		else {
			ret = true;
			auto map = GetMap(ScreenToGamePos(startDrag));
			if (map != NULL && map->type == MAP_CHEST_CLOSED) {
				map->param = current.type;
			}
			else {
				auto diff = startDrag - mousePos;
				if (diff.x < -25) {
					current.dir.x = 1;
				}
				else if (diff.x > 25) {
					current.dir.x = -1;
				}
				else {
					current.dir.x = 0;
				}
				if (diff.y < -25) {
					current.dir.y = 1;
				}
				else if (diff.y > 25) {
					current.dir.y = -1;
				}
				else {
					current.dir.y = 0;
				}

				if (current.dir.x != 0 || current.dir.y != 0) {
					current.frame = GetFlyingObjectSponeFrame();
					current.hp = createHP;
					current.size = createSize;
					current.speed = createSpeed;
					AddFlyingObjectSponer(current);
				}
			}
			current.type = FLYING_OBJECT_NONE;
		}

	}

	if (current.type != FLYING_OBJECT_NONE) {
		ret = true;
	}
	return ret;
}


Spone CheckCreateFlyingObject(D3DXVECTOR2 pos) {
	if (CheckSquare(pos, GetCreateFlyingObjectPos(-2), { CREATE_FLYING_OBJECT_WIDTH ,CREATE_FLYING_OBJECT_HEIGHT })) {
		isErase = true;
		return {};
	}
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		if (CheckSquare(pos, creates[i].initPos, { CREATE_FLYING_OBJECT_WIDTH ,CREATE_FLYING_OBJECT_HEIGHT })) {
			return creates[i];
		}
	}
	return {};
}
D3DXVECTOR2 GetCreateFlyingObjectPos(int type) {
	D3DXVECTOR2 pos;
	if (type % 2 == 0) {
		pos.x = CREATE_FLYING_OBJECT_ZERO_X + CREATE_FLYING_OBJECT_WIDTH + CREATE_FLYING_OBJECT_MARGIN;
	}
	else {
		pos.x = CREATE_FLYING_OBJECT_ZERO_X;
	}

	pos.y = CREATE_FLYING_OBJECT_ZERO_Y + (type / 2) * (CREATE_FLYING_OBJECT_HEIGHT + CREATE_FLYING_OBJECT_MARGIN);

	return pos;
}


void DrawFlyingObjectScreen(const Spone& map) {

	D3DXVECTOR2 size = D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT);

	D3DXVECTOR2 tPos = { 0,0 };
	D3DXVECTOR2 tSize = { CREATE_FLYING_OBJECT_TEXTURE_WIDTH,CREATE_FLYING_OBJECT_TEXTURE_HEIGHT };

	if (map.type == FLYING_OBJECT_UFO) {
		tSize *= 2;
	}
	if (map.type == FLYING_OBJECT_PURGE_BLOCK||map.type == FLYING_OBJECT_PLAYER_BLOCK||map.type == FLYING_OBJECT_ENEMY_BREAK_BLOCK_SECOND||map.type == FLYING_OBJECT_CHECKPOINT_OFF) {
		return;
	}

	DrawSprite(textureIds[map.type], map.initPos, 10, size, tPos, tSize);
}