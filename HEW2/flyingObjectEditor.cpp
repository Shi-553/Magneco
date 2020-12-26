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
#define CREATE_FLYING_OBJECT_ZERO_Y 150

#define CREATE_FLYING_OBJECT_WIDTH 50
#define CREATE_FLYING_OBJECT_HEIGHT 50

#define CREATE_FLYING_OBJECT_MARGIN 20

D3DXVECTOR2 GetCreateFlyingObjectPos(int type);
void DrawFlyingObjectScreen(const Spone& map);
Spone CheckCreateFlyingObject(D3DXVECTOR2 pos);
static bool isPlay = false;
static bool isDrag = false;

static Spone creates[FLYING_OBJECT_MAX];
static int textureIds[FLYING_OBJECT_MAX];

static Spone current;
static D3DXVECTOR2 startDrag;


void InitFlyingObjectEditor(){
	textureIds[FLYING_OBJECT_BLOCK ] = ReserveTextureLoadFile("texture/block01.png");
	textureIds[FLYING_OBJECT_ENEMY ] = ReserveTextureLoadFile("texture/jellyalien01.png");
	textureIds[FLYING_OBJECT_ENEMY_BREAK_BLOCK] = ReserveTextureLoadFile("texture/meteorite_1.png");
	textureIds[FLYING_OBJECT_UFO] = ReserveTextureLoadFile("texture/ufo.png");

	textureIds[FLYING_OBJECT_PLAYER_BLOCK] = ReserveTextureLoadFile("texture/block03.png");
	textureIds[FLYING_OBJECT_PURGE_BLOCK] = ReserveTextureLoadFile("texture/block03.png");
	textureIds[FLYING_OBJECT_ITEM_ADD_SPEED] = ReserveTextureLoadFile("texture/hane.png");
	textureIds[FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE] = ReserveTextureLoadFile("texture/maguneticPower.png");
	textureIds[FLYING_OBJECT_ITEM_CHAGE_BLOCK_UNBREAKABLE] = ReserveTextureLoadFile("texture/changeUnbreakable.png");


	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		creates[i].type = (FlyingObjectType)i;
		creates[i].initPos = GetCreateFlyingObjectPos(i);
	}
}
void UninitFlyingObjectEditor(){
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		ReleaseTexture(textureIds[i]);
	}
}

void DrawFlyingObjectEditor(){
	DrawFlyingObject();

	DEBUG_FONT_STATE s = {};
	s.pos = {500,0};
	DrawDebugFont(&s, "%d", GetFlyingObjectSponeFrame());

	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		DrawFlyingObjectScreen(creates[i]);
	}

	if (current.type != FLYING_OBJECT_NONE) {
		Spone s = current;
		s.initPos = GameToScreenPos(s.initPos)- D3DXVECTOR2(CREATE_FLYING_OBJECT_WIDTH, CREATE_FLYING_OBJECT_HEIGHT)/2;
		DrawFlyingObjectScreen(s);
	}
}
void UpdateFlyingObjectEditor(){
	DestroyUFO();

	if (TriggerInputLogger(MYVK_L)) {
		SetFlyingObjectSponerLoop(!GetFlyingObjectSponerLoop());
		GetFlyingObjects()->clear();
		SetFlyingObjectSponeFrame(0);
	}

	auto wheel = GetInputLoggerAxisAmountInt(MYVA_MW);
	//DebugPrintf("%d,\n", wheel);

	if (wheel != 0) {
		isPlay = false;
		SetFlyingObjectSponeFrame(GetFlyingObjectSponeFrame()+wheel/10);
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
bool CheckMouseFlyingObjectEditor(){
	auto mousePos = D3DXVECTOR2(GetInputLoggerAxisInt(MYVA_MX), GetInputLoggerAxisInt(MYVA_MY));
	bool ret=false;

	if (TriggerInputLogger(MYVK_LEFT_CLICK)) {
		startDrag = mousePos;
		isDrag = true;
	}
	if (current.type != FLYING_OBJECT_NONE && !isDrag) {
		current.initPos = ScreenToGamePos(mousePos);
	}
	if (ReleaseInputLogger(MYVK_LEFT_CLICK)) {
		isDrag = false;
		if (current.type == FLYING_OBJECT_NONE) {
			current = CheckCreateFlyingObject(mousePos);
		}
		else {
			ret = true;
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
			current.frame = GetFlyingObjectSponeFrame();
			AddFlyingObjectSponer(current);
			current.type = FLYING_OBJECT_NONE;
		}

	}

	if (current.type != FLYING_OBJECT_NONE) {
		ret = true;
	}
	return ret;
}


Spone CheckCreateFlyingObject(D3DXVECTOR2 pos) {
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		if (CheckSquare(pos,creates[i].initPos, { CREATE_FLYING_OBJECT_WIDTH ,CREATE_FLYING_OBJECT_HEIGHT})) {
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

	D3DXVECTOR2 size = D3DXVECTOR2( CREATE_FLYING_OBJECT_WIDTH,CREATE_FLYING_OBJECT_HEIGHT );

	D3DXVECTOR2 tPos = { 0,0 };
	D3DXVECTOR2 tSize = { CREATE_FLYING_OBJECT_TEXTURE_WIDTH,CREATE_FLYING_OBJECT_TEXTURE_HEIGHT };

	DrawSprite(textureIds[map.type], map.initPos, 10, size, tPos, tSize);
}