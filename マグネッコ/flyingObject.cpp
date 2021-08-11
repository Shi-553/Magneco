#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"
#include "map.h"
#include "time.h"
#include "npc.h"
#include "effect.h"
#include "player.h"
#include "flyingObjectSponer.h"
#include "sound.h"
// flyingObject描画範囲の加算分
#define FLYINGOBJECT_ADD_RANGE (5)

#define FLYINGOBJECT_TEXTURE_WIDTH (64)
#define FLYINGOBJECT_TEXTURE_HEIGHT (64)

#define FLYINGOBJECT_ITEM_TEXTURE_WIDTH (32)
#define FLYINGOBJECT_ITEM_TEXTURE_HEIGHT (32)

#define UFO_LIGHT_ANIMETION_TEXTURE (96)


static std::list<FlyingObject> flyingObjects;
static bool existsUFO = false;
static int flyingObjectTextureIds[FLYING_OBJECT_MAX];
static int blockAnimationTextureId;
static int ufoLightAnimationTextureId;
static int ufoBottomLightAnimationTextureId;
static int roadSignTextureId;

static int  nekoPanchiTextureId = TEXTURE_INVALID_ID;

static int  breakAlertTextureId = TEXTURE_INVALID_ID;
static int  breakNoticeTextureId = TEXTURE_INVALID_ID;

static int  checkPointGuideTextureId = TEXTURE_INVALID_ID;

static float breakAleartFrame = 0;

static bool isBreakAleartLeft = false;

void BreakBlock(INTVECTOR2 pos);

static int frame = 0;

static SponeId spone;

static int currentUID = 0;

std::list<FlyingObject>* GetFlyingObjects() {
	return &flyingObjects;
}

void AddFlyingObjects(FlyingObject* flyingObject) {
	if (flyingObject->type == FLYING_OBJECT_UFO) {// ufo画面上にない場合
		if (existsUFO) {
			return;
		}
		existsUFO = true;
		PlaySound(SOUND_LABEL_SE_UFO);
	}
	flyingObject->uid = currentUID;
	flyingObject->rad = 0;
	currentUID++;
	flyingObjects.push_back(*flyingObject);

}

void InitFlyingObject() {
	flyingObjects.clear();
	flyingObjectTextureIds[FLYING_OBJECT_BLOCK] = ReserveTextureLoadFile("texture/block/block02.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY] = ReserveTextureLoadFile("texture/enemy/jellyalienman_anime.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY_BREAK_BLOCK] = ReserveTextureLoadFile("texture/enemy/meteorite_1.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY_BREAK_BLOCK_SECOND] = ReserveTextureLoadFile("texture/enemy/meteorite_2.png");
	flyingObjectTextureIds[FLYING_OBJECT_UFO] = ReserveTextureLoadFile("texture/enemy/UFO.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY_SECOND] = ReserveTextureLoadFile("texture/enemy/jellyaliengirl_anime.png");
	flyingObjectTextureIds[FLYING_OBJECT_PLAYER_BLOCK] = ReserveTextureLoadFile("texture/block/block01.png");
	flyingObjectTextureIds[FLYING_OBJECT_PURGE_BLOCK] = ReserveTextureLoadFile("texture/block/nekopanchi_nikukyu.png");
	existsUFO = false;

	flyingObjectTextureIds[FLYING_OBJECT_ITEM_ADD_SPEED] = ReserveTextureLoadFile("texture/item/item_hane_anime.png");
	flyingObjectTextureIds[FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE] = ReserveTextureLoadFile("texture/item/item_hueru_anime.png");
	flyingObjectTextureIds[FLYING_OBJECT_CHECKPOINT_OFF] = ReserveTextureLoadFile("texture/block/point_block.png");
	blockAnimationTextureId = ReserveTextureLoadFile("texture/block/put_anime.png");
	ufoLightAnimationTextureId = ReserveTextureLoadFile("texture/enemy/beam_front.png");
	ufoBottomLightAnimationTextureId = ReserveTextureLoadFile("texture/enemy/beam_behind.png");
	roadSignTextureId = ReserveTextureLoadFile("texture/enemy/insekikuruyo.png");

	nekoPanchiTextureId = ReserveTextureLoadFile("texture/block/nekopanchi.png");
	breakAlertTextureId = ReserveTextureLoadFile("texture/enemy/breakAlert.png");
	breakNoticeTextureId = ReserveTextureLoadFile("texture/enemy/breakNotice.png");

	checkPointGuideTextureId = ReserveTextureLoadFile("texture/block/checkpoint_guide.png");

	frame = 0;
	breakAleartFrame = 0;
	currentUID = 0;
	isBreakAleartLeft = false;
}
void UninitFlyingObject() {
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		ReleaseTexture(flyingObjectTextureIds[i]);
	}
	ReleaseTexture(blockAnimationTextureId);
	ReleaseTexture(ufoLightAnimationTextureId);
	ReleaseTexture(ufoBottomLightAnimationTextureId);
	ReleaseTexture(roadSignTextureId);
	ReleaseTexture(nekoPanchiTextureId);
	ReleaseTexture(breakAlertTextureId);
	ReleaseTexture(breakNoticeTextureId);
}
void DrawFlyingObject(FlyingObject& flyingObject) {
	Player* player = GetPlayer();
	NPC* npc = GetNpc();
	auto textureId = flyingObjectTextureIds[flyingObject.type];


	if (flyingObject.type == FLYING_OBJECT_ENEMY_BREAK_BLOCK) {
		auto pos = flyingObject.trans.pos;

		if (GetMapType(pos) == MAP_ROCK) {
			auto p = INTVECTOR2(pos).ToD3DXVECTOR2();
			DrawGameSprite(breakNoticeTextureId, p, 10);
		}

		for (int i = 0; i < 100; i++) {

			if (GetMapType(pos) != MAP_BLOCK) {
				pos += flyingObject.dir;
				continue;
			}
			pos = INTVECTOR2(pos).ToD3DXVECTOR2();
			if (fabs(flyingObject.dir.x) > 0) {
				pos.x += breakAleartFrame;
			}
			else {
				pos.y += breakAleartFrame;
			}
			DrawGameSprite(breakAlertTextureId, pos, 10);
			break;
		}

	}

	if (flyingObject.type == FLYING_OBJECT_PURGE_BLOCK) {
		auto tPos = D3DXVECTOR2(
					FLYINGOBJECT_ITEM_TEXTURE_WIDTH * 2 * (frame / 12 % 6),
					0
		);
		auto size = flyingObject.size.ToD3DXVECTOR2();
		size.x *= 2;
		auto pos = flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0;
		pos.x -= flyingObject.size.x;
		auto d = D3DXVECTOR2(-1, 0);
		auto dr = D3DXVec2Dot(&d, &flyingObject.dir);
		if (dr > 1) {
			dr = 1;
		}
		if (dr < -1) {
			dr = -1;
		}
		auto rad = acosf(dr);
		if (d.x * flyingObject.dir.y - d.y * flyingObject.dir.x < 0) {
			rad = -rad;
		}
		auto cp = flyingObject.size.ToD3DXVECTOR2() / 2.0;
		cp.x += flyingObject.size.x;
		DrawGameSprite(nekoPanchiTextureId, pos, 50, size, tPos, D3DXVECTOR2(FLYINGOBJECT_ITEM_TEXTURE_WIDTH * 2, FLYINGOBJECT_ITEM_TEXTURE_HEIGHT), cp, rad);

	}

	if (flyingObject.isAnime) {
		DrawGameSprite(blockAnimationTextureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0, 50, flyingObject.size.ToD3DXVECTOR2(), { (float)(4 * player->putFrame / player->putRequiredFrame) * 32, 0 }, { 32, 32 });
		return;
	}
	if (flyingObject.type == FLYING_OBJECT_ITEM_ADD_SPEED || flyingObject.type == FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE || flyingObject.type == FLYING_OBJECT_ENEMY || flyingObject.type == FLYING_OBJECT_ENEMY_SECOND) {
		auto tPos = D3DXVECTOR2(
			FLYINGOBJECT_ITEM_TEXTURE_WIDTH * (frame / 12 % 8),
			0
		);

		DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0, 50, flyingObject.size.ToD3DXVECTOR2(), tPos, D3DXVECTOR2(FLYINGOBJECT_ITEM_TEXTURE_WIDTH, FLYINGOBJECT_ITEM_TEXTURE_HEIGHT));
	}
	else if (flyingObject.type == FLYING_OBJECT_UFO) {
		if (flyingObject.hp >= 3) {
			auto tPos = D3DXVECTOR2(
				FLYINGOBJECT_TEXTURE_WIDTH * (frame / 12 % 4),
				0
			);

			DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 1.6, 50, D3DXVECTOR2(1.3, 1.3), tPos, D3DXVECTOR2(FLYINGOBJECT_TEXTURE_WIDTH, FLYINGOBJECT_TEXTURE_HEIGHT));
		}
		else if (flyingObject.hp == 2) {
			auto tPos = D3DXVECTOR2(
				FLYINGOBJECT_TEXTURE_WIDTH * (frame / 12 % 4),
				FLYINGOBJECT_TEXTURE_HEIGHT
			);

			DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 1.6, 50, D3DXVECTOR2(1.3, 1.3), tPos, D3DXVECTOR2(FLYINGOBJECT_TEXTURE_WIDTH, FLYINGOBJECT_TEXTURE_HEIGHT));
		}
		else if (flyingObject.hp == 1) {
			auto tPos = D3DXVECTOR2(
				FLYINGOBJECT_TEXTURE_WIDTH * (frame / 12 % 4),
				FLYINGOBJECT_TEXTURE_HEIGHT * 2
			);

			DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 1.6, 50, D3DXVECTOR2(1.3, 1.3), tPos, D3DXVECTOR2(FLYINGOBJECT_TEXTURE_WIDTH, FLYINGOBJECT_TEXTURE_HEIGHT));
		}
		if (npc->contactUFO == true) {
			auto tPos = D3DXVECTOR2(
				FLYINGOBJECT_TEXTURE_WIDTH * (frame / 8 % 4),
				0
			);

			DrawGameSprite(ufoLightAnimationTextureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 1.6 + D3DXVECTOR2(-0.25, 0.5), 50, D3DXVECTOR2(1.8, 1.7), tPos, D3DXVECTOR2(FLYINGOBJECT_TEXTURE_WIDTH, UFO_LIGHT_ANIMETION_TEXTURE));
			DrawGameSprite(ufoBottomLightAnimationTextureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 1.6 + D3DXVECTOR2(-0.25, 0.5), 50, D3DXVECTOR2(1.8, 1.7), tPos, D3DXVECTOR2(FLYINGOBJECT_TEXTURE_WIDTH, UFO_LIGHT_ANIMETION_TEXTURE));
		}
	}
	else {
		DrawGameSprite(textureId,
			flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0,
			50, 
			flyingObject.size.ToD3DXVECTOR2(), 
			D3DXVECTOR2(0, 0),
			D3DXVECTOR2(FLYINGOBJECT_ITEM_TEXTURE_WIDTH, FLYINGOBJECT_ITEM_TEXTURE_HEIGHT),
			flyingObject.size.ToD3DXVECTOR2() / 2.0,
			3.141519f+ flyingObject.rad);
	}

	for (int i = 0; i < GetMapHeight(); i++)
	{
		for (int j = 0; j < GetMapWidth(); j++)
		{
			auto mapPos = INTVECTOR2(j, i);
			Map* map = GetMap(mapPos);
			if (map->type == MAP_CHAECKPOINT_OFF && MapFourDirectionsJudgment(mapPos))
			{
				auto tPos = D3DXVECTOR2(
					32 * (frame / 6 % 16),
					0
				);

				DrawGameSprite(checkPointGuideTextureId, D3DXVECTOR2(j + 0.1, i - 1.5), 100, D3DXVECTOR2(0.8, 1.5), tPos, D3DXVECTOR2(32, 64));
			}
		}
	}
	

}

void DrawFlyingObject() {

	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end(); itr++) {
		DrawFlyingObject(*itr);
	}
}
void UpdateFlyingObject() {
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
		if (UpdateFlyingObject(&*itr, itr->speed)) {
			DestrySpone(*itr);
			itr = flyingObjects.erase(itr);
		}
		else {
			itr++;
		}
	}

	frame++;
	if (!isBreakAleartLeft) {
		breakAleartFrame += 0.04;
		if (breakAleartFrame > 0.05) {
			isBreakAleartLeft = true;
		}
	}
	else {
		breakAleartFrame -= 0.04;
		if (breakAleartFrame < -0.05) {
			isBreakAleartLeft = false;
		}
	}
}
void BackFlyingObject(int frame) {
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
		if (UpdateFlyingObject(&*itr, -frame * itr->speed)) {
			if (itr->type == FLYING_OBJECT_UFO) {
				NPCDeleteUFO();
			}
			DestrySpone(*itr);
			itr = flyingObjects.erase(itr);
		}
		else {
			itr++;
		}
	}
}
bool UpdateFlyingObject(FlyingObject* flyingObject, float speed) {
	if (flyingObject->type == FLYING_OBJECT_ENEMY_BREAK_BLOCK) {
		flyingObject->rad += 0.05f;
	}
	if (flyingObject->type == FLYING_OBJECT_UFO) {
		flyingObject->dir = (GetNpc()->trans.pos + ADD_UFO_POS) - flyingObject->trans.pos;

		D3DXVec2Normalize(&flyingObject->dir, &flyingObject->dir);
	}
	else if (flyingObject->type == FLYING_OBJECT_PURGE_BLOCK) {
		D3DXVec2Normalize(&flyingObject->dir, &flyingObject->dir);
	}
	else {
		if (flyingObject->dir.x > 0) {
			flyingObject->dir.x = 1;
		}
		if (flyingObject->dir.x < 0) {
			flyingObject->dir.x = -1;
		}
		if (flyingObject->dir.y > 0) {
			flyingObject->dir.y = 1;
		}
		if (flyingObject->dir.y < 0) {
			flyingObject->dir.y = -1;
		}
	}
	flyingObject->trans.pos += flyingObject->dir * speed * GetDeltaTime();

	flyingObject->trans.UpdatePos();

	if (flyingObject->trans.pos.x > GetMapWidth() + FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.x < -GetMapWidth() - FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.y > GetMapHeight() + FLYINGOBJECT_ADD_RANGE ||
		flyingObject->trans.pos.y < -GetMapHeight() - FLYINGOBJECT_ADD_RANGE) {
		return true;
	}
	else {
		return false;
	}
}



void BreakBlock(FlyingObject& f) {
	if (f.type == FLYING_OBJECT_ENEMY_BREAK_BLOCK) {
		BreakBlock(f.trans.GetIntPos());
		/*BreakBlock(f.trans.GetIntPos() + INTVECTOR2(1, 0));
		BreakBlock(f.trans.GetIntPos() + INTVECTOR2(0, 1));
		BreakBlock(f.trans.GetIntPos() + INTVECTOR2(-1, 0));
		BreakBlock(f.trans.GetIntPos() + INTVECTOR2(0, -1));*/
	}
}
bool DamageFlyingObject(FlyingObject& f) {
	f.hp--;
	if (f.hp > 0) {
		return false;
	}
	if (f.type == FLYING_OBJECT_UFO) {
		existsUFO = false;
		StopSound(SOUND_LABEL_SE_UFO);
		NPCDeleteUFO();
	}
	
	DestrySpone(f);


	return true;
}
void BreakBlock(INTVECTOR2 pos) {
	if (auto m = GetMap(pos)) {
		if (m->type == MAP_BLOCK || m->type == MAP_ROCK) {
			m->type = MAP_BLOCK_NONE;
			BreakNotConnectBlock(pos + INTVECTOR2(0, 1));
			BreakNotConnectBlock(pos + INTVECTOR2(0, -1));
			BreakNotConnectBlock(pos + INTVECTOR2(1, 0));
			BreakNotConnectBlock(pos + INTVECTOR2(-1, 0));
		}
	}
}


bool IsFlyingObjectItem(FlyingObjectType type) {
	switch (type)
	{
	case FLYING_OBJECT_ITEM_ADD_SPEED:
	case FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE:
		//case FLYING_OBJECT_CHECKPOINT_OFF:
		return true;
	default:
		return false;
	}
}
bool IsFlyingObjectEnemy(FlyingObjectType type) {
	switch (type)
	{
	case FLYING_OBJECT_ENEMY:
	case FLYING_OBJECT_ENEMY_BREAK_BLOCK:
	case FLYING_OBJECT_ENEMY_BREAK_BLOCK_SECOND:
	case FLYING_OBJECT_UFO:
	case FLYING_OBJECT_ENEMY_SECOND:
		return true;
	default:
		return false;
	}
}
bool IsFlyingObjectBlock(FlyingObjectType type) {
	switch (type)
	{
	case FLYING_OBJECT_BLOCK:
	case FLYING_OBJECT_CHECKPOINT_OFF:
		return true;
	default:
		return false;
	}
}
bool IsFlyingObjectBreakBlockEnemy(FlyingObjectType type) {
	switch (type)
	{
	case FLYING_OBJECT_ENEMY_BREAK_BLOCK:
	case FLYING_OBJECT_ENEMY_BREAK_BLOCK_SECOND:
		return true;
	default:
		return false;
	}
}


int GetFlyingObjectTextureId(FlyingObjectType type) {
	if (type < FLYING_OBJECT_BLOCK || FLYING_OBJECT_MAX <= type) {
		return TEXTURE_INVALID_ID;
	}
	return flyingObjectTextureIds[(int)type];
}