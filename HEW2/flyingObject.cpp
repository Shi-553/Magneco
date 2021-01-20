#include <list>
#include "flyingObject.h"
#include "texture.h"
#include "myd3d.h"
#include "sprite.h"
#include "gameSrite.h"
#include "map.h"
#include "time.h"
#include "npc.h"
#include "player.h"
#include "flyingObjectSponer.h"

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
	}
	flyingObject->uid = currentUID;
	currentUID++;

	flyingObjects.push_back(*flyingObject);

}

void InitFlyingObject() {
	flyingObjects.clear();
	flyingObjectTextureIds[FLYING_OBJECT_BLOCK] = ReserveTextureLoadFile("texture/block/block02.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY] = ReserveTextureLoadFile("texture/enemy/jellyalien01.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY_BREAK_BLOCK] = ReserveTextureLoadFile("texture/enemy/meteorite_1.png");
	flyingObjectTextureIds[FLYING_OBJECT_UFO] = ReserveTextureLoadFile("texture/enemy/ufo_spritesheet.png");
	flyingObjectTextureIds[FLYING_OBJECT_ENEMY_SECOND] = ReserveTextureLoadFile("texture/enemy/jellyaliengirl01.png");
	flyingObjectTextureIds[FLYING_OBJECT_PLAYER_BLOCK] = ReserveTextureLoadFile("texture/block/block01.png");
	flyingObjectTextureIds[FLYING_OBJECT_PURGE_BLOCK] = ReserveTextureLoadFile("texture/block/block01.png");
	existsUFO = false;

	flyingObjectTextureIds[FLYING_OBJECT_ITEM_ADD_SPEED] = ReserveTextureLoadFile("texture/item/item_hane_anime.png");
	flyingObjectTextureIds[FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE] = ReserveTextureLoadFile("texture/item/item_hueru_anime.png");
	flyingObjectTextureIds[FLYING_OBJECT_CHECKPOINT_OFF] = ReserveTextureLoadFile("texture/block/point_block.png");
	blockAnimationTextureId = ReserveTextureLoadFile("texture/block/put_anime.png");
	ufoLightAnimationTextureId = ReserveTextureLoadFile("texture/enemy/beam_front.png");
	ufoBottomLightAnimationTextureId = ReserveTextureLoadFile("texture/enemy/beam_behind.png");
	roadSignTextureId = ReserveTextureLoadFile("texture/enemy/insekikuruyo.png");

	frame = 0;
	currentUID = 0;
}
void UninitFlyingObject() {
	for (int i = FLYING_OBJECT_BLOCK; i < FLYING_OBJECT_MAX; i++) {
		ReleaseTexture(flyingObjectTextureIds[i]);
	}
	ReleaseTexture(blockAnimationTextureId);
	ReleaseTexture(ufoLightAnimationTextureId);
	ReleaseTexture(ufoBottomLightAnimationTextureId);
	ReleaseTexture(roadSignTextureId);
}
void DrawFlyingObject(FlyingObject& flyingObject) {
	Player* player = GetPlayer();
	NPC* npc = GetNpc();
	auto textureId = flyingObjectTextureIds[flyingObject.type];


	if (flyingObject.isAnime) {
		DrawGameSprite(blockAnimationTextureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0, 50, flyingObject.size.ToD3DXVECTOR2(), { (float)(4 * player->putFrame / DEFAULT_PUT_REQUIRED_FRAME) * 32, 0 }, { 32, 32 });
		return;
	}
	if (flyingObject.type == FLYING_OBJECT_ITEM_ADD_SPEED || flyingObject.type == FLYING_OBJECT_ITEM_ADD_MAGNETIC_FORCE) {
		auto tPos = D3DXVECTOR2(
			FLYINGOBJECT_ITEM_TEXTURE_WIDTH * (frame / 12 % 8),
			0
		);

		DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0, 50, flyingObject.size.ToD3DXVECTOR2(), tPos, D3DXVECTOR2(FLYINGOBJECT_ITEM_TEXTURE_WIDTH, FLYINGOBJECT_ITEM_TEXTURE_HEIGHT));
	}
	else if (flyingObject.type == FLYING_OBJECT_UFO) {
		auto tPos = D3DXVECTOR2(
			FLYINGOBJECT_TEXTURE_WIDTH * (frame / 12 % 4),
			0
		);

		DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 1.6, 50, D3DXVECTOR2(1.3, 1.3), tPos, D3DXVECTOR2(FLYINGOBJECT_TEXTURE_WIDTH, FLYINGOBJECT_TEXTURE_HEIGHT));
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
		DrawGameSprite(textureId, flyingObject.trans.pos - flyingObject.size.ToD3DXVECTOR2() / 2.0, 50, flyingObject.size.ToD3DXVECTOR2());
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
			itr = flyingObjects.erase(itr);
		}
		else {
			itr++;
		}
	}

	frame++;
}
void BackFlyingObject(int frame) {
	for (auto itr = flyingObjects.begin(); itr != flyingObjects.end();) {
		if (UpdateFlyingObject(&*itr, -frame * itr->speed)) {
			if (itr->type == FLYING_OBJECT_UFO) {
				DestroyUFO();
			}
			itr = flyingObjects.erase(itr);
		}
		else {
			itr++;
		}
	}
}
bool UpdateFlyingObject(FlyingObject* flyingObject, float speed) {
	if (flyingObject->type == FLYING_OBJECT_UFO) {
		flyingObject->dir = (GetNpc()->trans.pos + ADD_UFO_POS) - flyingObject->trans.pos;

	}
	auto nomal = flyingObject->dir;
	D3DXVec2Normalize(&nomal, &nomal);
	flyingObject->trans.pos += nomal * speed * GetDeltaTime();

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

void DestroyUFO() {
	existsUFO = false;
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