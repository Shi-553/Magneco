﻿#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "myd3d.h"
#include "gameSrite.h"
#include "npc.h"
#include <d3dx9.h>
#include "map.h"
#include <queue>
#include <stack>
#include "sceneManager.h"
#include "InputLogger.h"
#include "font.h"
#include <string>

typedef struct MapLabel {
	INTVECTOR2 pos;
	int label, notBlockCount;
};

INTVECTOR2 FindNearestBlock();
void FourDirFindNearestBlock(std::deque<MapLabel>* mapQueue, MapLabel* label, MapLabel* nearest);

bool FindShortestPath();
void FourDir(std::queue<MapLabel>* mapQueue, MapLabel* label);
int& GetMapLabel(int y, int x);
bool NPCRespone();

#define NPC_FONT_WIDTH 30
#define NPC_FONT_HIGHT 64

#define NPC_TEXTURE_WIDTH 64
#define NPC_TEXTURE_HEIGHT 64

#define BEACON_TEXTURE_WIDTH 32
#define BEACON_TEXTURE_HEIGHT 64

#define NPC_SIZE_WIDTH 1.76
#define NPC_SIZE_HEIGHT 1.76

static int beaconTextureId = TEXTURE_INVALID_ID;

static int npcTextureIdWait = TEXTURE_INVALID_ID;
static int npcTextureIdMove = TEXTURE_INVALID_ID;
static int npcTextureIdShadow = TEXTURE_INVALID_ID;
static NPC npc;

static int* mapLabelList = NULL;
std::stack<INTVECTOR2> nextPosQueue;

static INTVECTOR2 nextPos;
static INTVECTOR2 dir;
static INTVECTOR2 gBeaconPos;

static int  npcTextureVertical = 0;

static LPD3DXFONT font = NULL;
void InitNPC() {
	beaconTextureId = ReserveTextureLoadFile("texture/npc/beacon_anime.png");

	npcTextureIdWait = ReserveTextureLoadFile("texture/npc/spr_rose_idle.png");
	npcTextureIdMove = ReserveTextureLoadFile("texture/npc/spr_rose_walk.png");
	npcTextureIdShadow = ReserveTextureLoadFile("texture/npc/spr_shadow.png");
	npc.speed = 1;
	npc.trans.Init(2, 7);
	npc.frame = 30;
	npc.aniFrame = 0;
	npc.isMove = false;
	npc.takeOutFrame = 0;
	npc.responePos = npc.trans.pos;
	npc.size = { 0.5,0.5 };
	npcTextureVertical = 0;
	nextPos = npc.trans.GetIntPos();
	dir = INTVECTOR2(0, 0);
	gBeaconPos = npc.trans.GetIntPos();
	while (!nextPosQueue.empty()) {
		nextPosQueue.pop();
	}
	MyCreateFont(NPC_FONT_HIGHT, NPC_FONT_WIDTH, &font);
}

void UninitNPC() {
	ReleaseTexture(npcTextureIdWait);

	if (mapLabelList != NULL) {
		delete[] mapLabelList;
		mapLabelList = NULL;
	}
	font->Release();
}

void UpdateNPC() {

	npc.aniFrame++;

	if (NPCRespone()) {
		npc.frame = 0;
		dir = INTVECTOR2(0, 0);
		npc.isMove = false;
		nextPos = npc.trans.GetIntPos();
		gBeaconPos = npc.trans.GetIntPos();
		while (!nextPosQueue.empty()) {
			nextPosQueue.pop();
		}
	}
	if (npc.frame >= 30) {

		npc.trans.pos = nextPos.ToD3DXVECTOR2();
		npc.trans.UpdatePos();


		if (nextPosQueue.empty()) {
			npc.isMove = false;
			return;
		}

		npc.isMove = true;

		dir = nextPosQueue.top() - npc.trans.GetIntPos();

		if (dir.y == 1) {
			npcTextureVertical = 0;
		}
		if (dir.y == -1)
		{
			npcTextureVertical = NPC_TEXTURE_HEIGHT;
		}
		if (dir.x == -1)
		{
			npcTextureVertical = NPC_TEXTURE_HEIGHT * 2;
		}
		if (dir.x == 1) {
			npcTextureVertical = NPC_TEXTURE_HEIGHT * 3;
		}

		nextPos = nextPosQueue.top();

		nextPosQueue.pop();

		npc.frame = 0;
	}

	npc.trans.pos += dir.ToD3DXVECTOR2() / 30;


	npc.frame++;

}

void DrawNPC() {

	auto drawingPos = npc.trans.pos;
	drawingPos.x -= 0.39f;
	drawingPos.y -= 1.05f;

	if (!npc.isMove) {
		auto tPos = D3DXVECTOR2(
			NPC_TEXTURE_WIDTH * (npc.aniFrame / 7 % 15),
			npcTextureVertical
		);

		DrawGameSprite(npcTextureIdWait, drawingPos, 30, D3DXVECTOR2(NPC_SIZE_WIDTH, NPC_SIZE_HEIGHT), tPos, D3DXVECTOR2(NPC_TEXTURE_WIDTH, NPC_TEXTURE_HEIGHT));
		DrawGameSprite(npcTextureIdShadow, drawingPos, 30, D3DXVECTOR2(NPC_SIZE_WIDTH, NPC_SIZE_HEIGHT));
	}
	else
	{
		auto tBeaconPos = D3DXVECTOR2(
			BEACON_TEXTURE_WIDTH * (npc.aniFrame / 8 % 12),
			0
		);
		
		DrawGameSprite(beaconTextureId, gBeaconPos.ToD3DXVECTOR2() + D3DXVECTOR2(0.0f, -1.0f), 30, D3DXVECTOR2(1, 2), tBeaconPos, D3DXVECTOR2(BEACON_TEXTURE_WIDTH, BEACON_TEXTURE_HEIGHT));

		auto tPos = D3DXVECTOR2(
			NPC_TEXTURE_WIDTH * (npc.aniFrame / 6 % 6),
			npcTextureVertical
		);

		DrawGameSprite(npcTextureIdMove, drawingPos, 30, D3DXVECTOR2(NPC_SIZE_WIDTH, NPC_SIZE_HEIGHT), tPos, D3DXVECTOR2(NPC_TEXTURE_WIDTH, NPC_TEXTURE_HEIGHT));
		DrawGameSprite(npcTextureIdShadow, drawingPos, 30, D3DXVECTOR2(NPC_SIZE_WIDTH, NPC_SIZE_HEIGHT));
	}

	if (npc.takeOutFrame > 0) {
		auto string=std::to_string((TAKE_OUT_FRAME_LIMIT -npc.takeOutFrame+1)/100);
		RECT r;
		auto g = npc.trans.pos + D3DXVECTOR2(0.5, 0.5);
		auto screenPos = GameToScreenPos(g);
		r.left = screenPos.x;
		r.right = screenPos.x;
		r.top = screenPos.y-150;
		
		font->DrawTextA(NULL, string.c_str(), string.size(), &r, DT_CENTER | DT_NOCLIP, D3DCOLOR_RGBA(255,255,255,255));
	}
}

void UpdateNPCShortestPath(INTVECTOR2 beaconPos) {
	if (!npc.isMove) {
		gBeaconPos = beaconPos;
		UpdateNPCShortestPath();
	}
}

void UpdateNPCShortestPath() {
	if (gBeaconPos == npc.trans.GetIntPos()) {
		return;
	}
	for (int i = 0; i < GetMapHeight(); i++)
	{
		for (int j = 0; j < GetMapWidth(); j++)
		{
			GetMapLabel(i, j) = 0;
		}
	}
	while (!nextPosQueue.empty()) {
		nextPosQueue.pop();
	}

	INTVECTOR2 current;
	if (FindShortestPath()) {
		current = gBeaconPos;

	}
	else {
		current = FindNearestBlock();
	}
	while (true) {
		if (npc.trans.GetIntPos() == current) {
			break;
		}
		nextPosQueue.push(current);

		int label = GetMapLabel(current.y, current.x);

		auto right = current;
		right.x++;
		auto mapType = GetMapType(right);
		if (GetMapLabel(right.y, right.x) == label - 1 && CanGoNPCMapType(mapType)) {
			current = right;
			continue;
		}
		auto bottom = current;
		bottom.y++;
		mapType = GetMapType(bottom);
		if (GetMapLabel(bottom.y, bottom.x) == label - 1 && CanGoNPCMapType(mapType)) {
			current = bottom;
			continue;
		}
		auto left = current;
		left.x--;
		mapType = GetMapType(left);
		if (GetMapLabel(left.y, left.x) == label - 1 && CanGoNPCMapType(mapType)) {
			current = left;
			continue;
		}
		auto top = current;
		top.y--;
		mapType = GetMapType(top);
		if (GetMapLabel(top.y, top.x) == label - 1 && CanGoNPCMapType(mapType)) {
			current = top;
			continue;
		}
	}
}
INTVECTOR2 FindNearestBlock() {

	std::deque<MapLabel> mapQueue;
	mapQueue.push_back({ gBeaconPos,-1,0 });

	GetMapLabel(gBeaconPos.y, gBeaconPos.x) = -1;

	MapLabel nearest = {
		npc.trans.GetIntPos(),
		-10000,
		10000
	};

	while (!mapQueue.empty()) {
		auto mapLabel = mapQueue.front();
		mapQueue.pop_front();

		mapLabel.label--;
		if (mapLabel.notBlockCount > nearest.notBlockCount || (mapLabel.notBlockCount == nearest.notBlockCount && mapLabel.label <= nearest.label)) {
			continue;
		}

		auto right = mapLabel;
		right.pos.x++;
		FourDirFindNearestBlock(&mapQueue, &right, &nearest);

		auto bottom = mapLabel;
		bottom.pos.y++;
		FourDirFindNearestBlock(&mapQueue, &bottom, &nearest);

		auto left = mapLabel;
		left.pos.x--;
		FourDirFindNearestBlock(&mapQueue, &left, &nearest);

		auto top = mapLabel;
		top.pos.y--;
		FourDirFindNearestBlock(&mapQueue, &top, &nearest);

	}

	return nearest.pos;
}
void FourDirFindNearestBlock(std::deque<MapLabel>* mapQueue, MapLabel* label, MapLabel* nearest) {
	auto mapType = GetMapType(label->pos);
	//そこが到達可能なとき
	if (GetMapLabel(label->pos.y, label->pos.x) > 0) {
		//置かないといけないブロックが今より少ないかどうかと、同じならビーコンからの距離が短いかどうか
		if (label->notBlockCount < nearest->notBlockCount || (label->notBlockCount == nearest->notBlockCount && label->label > nearest->label)) {
			*nearest = *label;
		}
		return;
	}
	if (mapType == MAP_BLOCK_NONE) {
		label->notBlockCount++;
	}
	if (CanGoNPCMapType(mapType) || mapType == MAP_BLOCK_NONE) {

		if (GetMapLabel(label->pos.y, label->pos.x) == 0) {
			mapQueue->push_back(*label);

			GetMapLabel(label->pos.y, label->pos.x) = label->label;
		}
		else {
			for (auto itr = mapQueue->begin(); itr != mapQueue->end(); itr++) {
				if (itr->pos == label->pos &&
					(itr->notBlockCount > label->notBlockCount || (itr->notBlockCount == label->notBlockCount && itr->label < label->label))) {
					itr->notBlockCount = label->notBlockCount;
					itr->label = label->label;
					GetMapLabel(label->pos.y, label->pos.x) = label->label;
				}
			}
			if (nearest->pos == label->pos &&
				(nearest->notBlockCount > label->notBlockCount || (nearest->notBlockCount == label->notBlockCount && nearest->label < label->label))) {
				nearest->notBlockCount = label->notBlockCount;
				nearest->label = label->label;
				GetMapLabel(label->pos.y, label->pos.x) = label->label;
			}

		}
	}
}

bool FindShortestPath() {
	auto intPos = npc.trans.GetIntPos();

	std::queue<MapLabel> mapQueue;
	mapQueue.push({ intPos,1 });

	GetMapLabel(intPos.y, intPos.x) = 1;

	while (!mapQueue.empty()) {
		auto mapLabel = mapQueue.front();
		mapQueue.pop();

		mapLabel.label++;

		auto right = mapLabel;
		right.pos.x++;
		FourDir(&mapQueue, &right);
		if (right.pos == gBeaconPos) {
			return true;
		}

		auto bottom = mapLabel;
		bottom.pos.y++;
		FourDir(&mapQueue, &bottom);
		if (bottom.pos == gBeaconPos) {
			return true;
		}

		auto left = mapLabel;
		left.pos.x--;
		FourDir(&mapQueue, &left);
		if (left.pos == gBeaconPos) {
			return true;
		}

		auto top = mapLabel;
		top.pos.y--;
		FourDir(&mapQueue, &top);
		if (top.pos == gBeaconPos) {
			return true;
		}

	}

	return false;
}
void FourDir(std::queue<MapLabel>* mapQueue, MapLabel* label) {
	auto mapType = GetMapType(label->pos);
	if (CanGoNPCMapType(mapType) && GetMapLabel(label->pos.y, label->pos.x) == 0) {
		mapQueue->push(*label);

		GetMapLabel(label->pos.y, label->pos.x) = label->label;
	}

}

int& GetMapLabel(int y, int x) {
	return mapLabelList[y * GetMapWidth() + x];
}

void SecureMapLabelList() {
	if (mapLabelList != NULL) {
		delete[] mapLabelList;
		mapLabelList = NULL;
	}

	mapLabelList = new int[GetMapHeight() * GetMapWidth()];
}

NPC* GetNpc() {
	return &npc;
}


bool NPCExport(FILE* fp) {

	INTVECTOR2 pos = npc.trans.GetIntPos();
	//	ファイルへの書き込み処理
	fwrite(&pos, sizeof(INTVECTOR2), 1, fp);

	return true;
}


bool NPCImport(FILE* fp) {
	INTVECTOR2 pos;
	//	ファイルへの読み込み処理
	fread(&pos, sizeof(INTVECTOR2), 1, fp);
	npc.trans.Init(pos);
	nextPos = npc.trans.GetIntPos();

	return true;
}

void SetNPCResponePos(INTVECTOR2 pos) {
	npc.responePos = pos;
}

bool NPCRespone() {
	auto mapType = GetMapType(npc.trans.GetIntPos());
	if (!CanGoNPCMapType(mapType)) {
		npc.trans.Init(npc.responePos);
		return true;
	}
	return false;

}