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

#define CREATE_FLYING_OBJECT_TEXTURE_WIDTH 32
#define CREATE_FLYING_OBJECT_TEXTURE_HEIGHT 32


#define CREATE_FLYING_OBJECT_ZERO_X 1050
#define CREATE_FLYING_OBJECT_ZERO_Y 250

#define CREATE_FLYING_OBJECT_WIDTH 50
#define CREATE_FLYING_OBJECT_HEIGHT 50

#define CREATE_FLYING_OBJECT_MARGIN 20

#define CREATE_PROPERTY_X 1000
#define CREATE_HP_Y 40
#define CREATE_SIZEX_Y 70
#define CREATE_SIZEY_Y 100
#define CREATE_SPPED_Y 130
#define CREATE_PROPERTY_MARGIN D3DXVECTOR2(20,5)


bool CheckWheelProperty(D3DXVECTOR2 pos, D3DXVECTOR2 addPos);
bool CheckChangeProperty(D3DXVECTOR2 pos, D3DXVECTOR2 addPos);


D3DXVECTOR2 GetCreateFlyingObjectPos(int type);
void DrawFlyingObjectScreen(const Spone& map);
Spone CheckCreateFlyingObject(D3DXVECTOR2 pos);
void SetSpeed(float speed);

static bool isPlay = false;
static bool isDrag = false;

static Spone creates[FLYING_OBJECT_MAX];
static int textureIds[FLYING_OBJECT_MAX];

static int eraseTextureId = TEXTURE_INVALID_ID;
static bool isErase = false;

static Spone current;
static D3DXVECTOR2 startDrag;

static int createHP = 1;
static INTVECTOR2 createSize = { 1,1 };
static float createSpeed = 1.0f;

void SetSpeed(float speed) {
	createSpeed = speed;
	createSpeed *= 10;
	createSpeed = roundf(createSpeed);
	createSpeed /= 10;

	if (createSpeed < 0) {
		createSpeed = 0;
	}
}

void InitFlyingObjectEditor() {
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++)
	{
		textureIds[i] = GetFlyingObjectTextureId((FlyingObjectType)i);
	}


	eraseTextureId = ReserveTextureLoadFile("texture/MAP_BLOCK_NONE_ERASE.png");


	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		creates[i].type = (FlyingObjectType)i;
		creates[i].initPos = GetCreateFlyingObjectPos(i);
		creates[i].size = { 1,1 };
	}
}
void UninitFlyingObjectEditor() {
	ReleaseTexture(eraseTextureId);
}

void DrawFlyingObjectEditor() {
	DrawFlyingObject();

	DEBUG_FONT_STATE s = {};
	s.pos = { 500,0 };
	DrawDebugFont(&s, "%d", GetFlyingObjectSponeFrame());

	s.scale = 0.5;
	s.pos = { CREATE_PROPERTY_X,0 };
	DrawDebugFont(&s, "CREATE");
	s.pos = { CREATE_PROPERTY_X,CREATE_HP_Y };
	DrawDebugFont(&s, "HP  <   >");
	s.pos = { CREATE_PROPERTY_X + DEBUG_FONT_WIDTH * 6 / 2,CREATE_HP_Y };
	DrawDebugFont(&s, "%d", createHP);

	s.pos = { CREATE_PROPERTY_X,CREATE_SIZEX_Y };
	DrawDebugFont(&s, "SX  <   >");
	s.pos = { CREATE_PROPERTY_X + DEBUG_FONT_WIDTH * 6 / 2,CREATE_SIZEX_Y };
	DrawDebugFont(&s, "%d", createSize.x);
	s.pos = { CREATE_PROPERTY_X,CREATE_SIZEY_Y };
	DrawDebugFont(&s, "SY  <   >");
	s.pos = { CREATE_PROPERTY_X + DEBUG_FONT_WIDTH * 6 / 2,CREATE_SIZEY_Y };
	DrawDebugFont(&s, "%d", createSize.y);

	s.pos = { CREATE_PROPERTY_X,CREATE_SPPED_Y };
	DrawDebugFont(&s, "SPD <   >");
	s.pos = { CREATE_PROPERTY_X + DEBUG_FONT_WIDTH * 6 / 2,CREATE_SPPED_Y };
	DrawDebugFont(&s, "%f", createSpeed);

	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		DrawFlyingObjectScreen(creates[i]);
	}

	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));

	D3DXVECTOR2 size = D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT);

	D3DXVECTOR2 tPos = { 0,0 };
	D3DXVECTOR2 tSize = { CREATE_FLYING_OBJECT_TEXTURE_WIDTH,CREATE_FLYING_OBJECT_TEXTURE_HEIGHT };

	DrawSprite(eraseTextureId, GetCreateFlyingObjectPos(-2), 10, size, tPos, tSize);

	if (isErase) {
		DrawSprite(eraseTextureId, mousePos - D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT) / 2, 10, size, tPos, tSize);
	}
	else
		if (current.type != FLYING_OBJECT_NONE) {
			Spone s = current;
			s.initPos = GameToScreenPos(s.initPos) - D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT) / 2;
			DrawFlyingObjectScreen(s);
		}
}
void UpdateFlyingObjectEditor() {
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));

	DestroyUFO();

	if (TriggerInputLogger(MYVK_L)) {
		SetFlyingObjectSponerLoop(!GetFlyingObjectSponerLoop());
		GetFlyingObjects()->clear();
		SetFlyingObjectSponeFrame(0);
	}

	auto wheel = GetInputLoggerAxisAmountInt(MYVA_MW);
	//DebugPrintf("%d,\n", wheel);

	if (wheel != 0) {
		auto base = PressInputLogger(MYVK_SHIFT) ? 5 : 1;

		if (CheckWheelProperty(mousePos, D3DXVECTOR2(0, CREATE_HP_Y))) {
			createHP += (wheel / 120) * base;
			if (createHP < 1) {
				createHP = 1;
			}
		}
		else if (CheckWheelProperty(mousePos, D3DXVECTOR2(0, CREATE_SIZEX_Y))) {
			createSize.x += (wheel / 120) * base;
			if (createSize.x < 1) {
				createSize.x = 1;
			}
		}
		else if (CheckWheelProperty(mousePos, D3DXVECTOR2(0, CREATE_SIZEY_Y))) {
			createSize.y += (wheel / 120)* base;
			if (createSize.y < 1) {
				createSize.y = 1;
			}
		}
		else if (CheckWheelProperty(mousePos, D3DXVECTOR2(0, CREATE_SPPED_Y))) {
			SetSpeed(createSpeed + (wheel / 120) / 10.0f * base);
			
		}
		else {
			isPlay = false;
			SetFlyingObjectSponeFrame(GetFlyingObjectSponeFrame() + (wheel / 10) * base);
		}
	}

	if (TriggerInputLogger(MYVK_START_STOP)) {
		isPlay = !isPlay;
	}

	if (TriggerInputLogger(MYVK_FIRST_START)) {
		isPlay = true;
		GetFlyingObjects()->clear();
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
		auto base = PressInputLogger(MYVK_SHIFT) ? 5 : 1;
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 8 / 2, CREATE_HP_Y))) {
			createHP+= base;
			return true;
		}
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 4 / 2, CREATE_HP_Y))) {
			if (createHP > 1) {
				createHP-= base;
			}
			return true;
		}
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 8 / 2, CREATE_SIZEX_Y))) {
			createSize.x+= base;
			return true;
		}
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 4 / 2, CREATE_SIZEX_Y))) {
			if (createSize.x > 1) {
				createSize.x-= base;
			}
			return true;
		}
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 8 / 2, CREATE_SIZEY_Y))) {
			createSize.y+= base;
			return true;
		}
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 4 / 2, CREATE_SIZEY_Y))) {
			if (createSize.y > 1) {
				createSize.y-= base;
			}
			return true;
		}
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 8 / 2, CREATE_SPPED_Y))) {
			SetSpeed(createSpeed+ 0.1f * base);
			return true;
		}
		if (CheckChangeProperty(mousePos, D3DXVECTOR2(DEBUG_FONT_WIDTH * 4 / 2, CREATE_SPPED_Y))) {
			SetSpeed(createSpeed - 0.1f * base);
			return true;
		}

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

bool CheckChangeProperty(D3DXVECTOR2 pos, D3DXVECTOR2 addPos) {
	D3DXVECTOR2 upPos = { CREATE_PROPERTY_X + addPos.x,addPos.y };
	D3DXVECTOR2 size = { DEBUG_FONT_WIDTH / 2,DEBUG_FONT_HEIGHT / 2 };

	return CheckSquare(pos, upPos - CREATE_PROPERTY_MARGIN / 2, size + CREATE_PROPERTY_MARGIN);
}
bool CheckWheelProperty(D3DXVECTOR2 pos, D3DXVECTOR2 addPos) {
	D3DXVECTOR2 hpPos = { CREATE_PROPERTY_X + addPos.x ,addPos.y };
	D3DXVECTOR2 size = { DEBUG_FONT_WIDTH * 9 / 2,DEBUG_FONT_HEIGHT / 2 };

	return CheckSquare(pos, hpPos - CREATE_PROPERTY_MARGIN / 2, size + CREATE_PROPERTY_MARGIN);
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

	DrawSprite(textureIds[map.type], map.initPos, 10, size, tPos, tSize);
}