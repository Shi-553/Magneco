#include "config.h"
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

typedef struct MapLabel {
	INTVECTOR2 pos;
	int label, notBlockCount;
};

INTVECTOR2 FindNearestBlock();
void FourDirFindNearestBlock(std::deque<MapLabel>* mapQueue, MapLabel* label, MapLabel* nearest);

bool FindShortestPath();
void FourDir(std::queue<MapLabel>* mapQueue, MapLabel* label);


#define NPC_TEXTURE_WIDTH 64
#define NPC_TEXTURE_HEIGHT 64

#define NPC_SIZE_WIDTH 88
#define NPC_SIZE_HEIGHT 88

static int beaconTextureId = TEXTURE_INVALID_ID;

static int npcTextureIdWait = TEXTURE_INVALID_ID;
static int npcTextureIdMove = TEXTURE_INVALID_ID;
static int npcTextureIdShadow = TEXTURE_INVALID_ID;
static NPC npc;

static int mapLabelList[MAPCHIP_HEIGHT][MAPCHIP_WIDTH];
std::stack<INTVECTOR2> nextPosQueue;

static INTVECTOR2 nextPos;
static INTVECTOR2 dir;
static INTVECTOR2 gBeaconPos;

static int  npcTextureVertical = 0;

void InitNPC() {
	beaconTextureId = ReserveTextureLoadFile("texture/beacon01.png");

	npcTextureIdWait = ReserveTextureLoadFile("texture/spr_rose_idle.png");
	npcTextureIdMove = ReserveTextureLoadFile("texture/spr_rose_walk.png");
	npcTextureIdShadow = ReserveTextureLoadFile("texture/spr_shadow.png");
	npc.speed = 1;
	npc.trans.Init(2, 7);
	npc.frame = 30;
	npc.aniFrame = 0;
	npc.isMove = false;
	npcTextureVertical = 0;
	nextPos = npc.trans.GetIntPos();
	dir = INTVECTOR2(0, 0);
	gBeaconPos = npc.trans.GetIntPos();
	while (!nextPosQueue.empty()) {
		nextPosQueue.pop();
	}
}

void UninitNPC() {
	ReleaseTexture(npcTextureIdWait);
	ReleaseTexture(beaconTextureId);
}

void UpdateNPC() {

	npc.aniFrame++;

	if (npc.frame >= 30) {

		npc.trans.pos = nextPos.ToD3DXVECTOR2();
		npc.trans.UpdatePos();

		if (GetMapType(npc.trans.GetIntPos()) == MAP_GOAL) {
			GoNextScene(GameClearScene);
		}


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
	npc.trans.UpdatePos();

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
		auto tPos = D3DXVECTOR2(
			NPC_TEXTURE_WIDTH * (npc.aniFrame / 6 % 6),
			npcTextureVertical
		);

		DrawGameSprite(npcTextureIdMove, drawingPos, 30, D3DXVECTOR2(NPC_SIZE_WIDTH, NPC_SIZE_HEIGHT), tPos, D3DXVECTOR2(NPC_TEXTURE_WIDTH, NPC_TEXTURE_HEIGHT));
		DrawGameSprite(npcTextureIdShadow, drawingPos, 30, D3DXVECTOR2(NPC_SIZE_WIDTH, NPC_SIZE_HEIGHT));
	}
	
}

void UpdateNPCShortestPath(D3DXVECTOR2 beaconPos) {
	gBeaconPos = beaconPos;
	UpdateNPCShortestPath();
}

void UpdateNPCShortestPath() {
	if (gBeaconPos == npc.trans.GetIntPos()) {
		return;
	}
	for (int i = 0; i < MAPCHIP_HEIGHT; i++)
	{
		for (int j = 0; j < MAPCHIP_WIDTH; j++)
		{
			mapLabelList[i][j] = 0;
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

		int label = mapLabelList[current.y][current.x];

		auto right = current;
		right.x ++;
		auto mapType = GetMapType(right);
		if (mapLabelList[right.y][right.x] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current = right;
			continue;
		}
		auto bottom = current;
		bottom.y++;
		mapType = GetMapType(bottom);
		if (mapLabelList[bottom.y ][bottom.x] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current = bottom;
			continue;
		}
		auto left = current;
		left.x--;
		mapType = GetMapType(left);
		if (mapLabelList[left.y][left.x] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current = left;
			continue;
		}
		auto top = current;
		top.y--;
		mapType = GetMapType(top);
		if (mapLabelList[top.y][top.x] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current = top;
			continue;
		}
	}
}
INTVECTOR2 FindNearestBlock() {

	std::deque<MapLabel> mapQueue;
	mapQueue.push_back({ gBeaconPos,-1,0 });

	mapLabelList[gBeaconPos.y][gBeaconPos.x] = -1;

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
	if (mapLabelList[label->pos.y][label->pos.x] > 0) {
		//置かないといけないブロックが今より少ないかどうかと、同じならビーコンからの距離が短いかどうか
		if (label->notBlockCount < nearest->notBlockCount || (label->notBlockCount == nearest->notBlockCount && label->label > nearest->label)) {
			*nearest = *label;
		}
		return;
	}
	if (mapType == MAP_BLOCK_NONE) {
		label->notBlockCount++;
	}
	if (mapType == MAP_BLOCK || mapType == MAP_GOAL || mapType == MAP_BLOCK_NONE) {

		if (mapLabelList[label->pos.y][label->pos.x] == 0) {
			mapQueue->push_back(*label);

			mapLabelList[label->pos.y][label->pos.x] = label->label;
		}
		else {
			for (auto itr = mapQueue->begin(); itr != mapQueue->end(); itr++) {
				if (itr->pos == label->pos &&
					(itr->notBlockCount > label->notBlockCount || (itr->notBlockCount == label->notBlockCount && itr->label < label->label))) {
					itr->notBlockCount = label->notBlockCount;
					itr->label = label->label;
					mapLabelList[label->pos.y][label->pos.x] = label->label;
				}
			}
			if (nearest->pos == label->pos &&
				(nearest->notBlockCount > label->notBlockCount || (nearest->notBlockCount == label->notBlockCount && nearest->label < label->label))) {
				nearest->notBlockCount = label->notBlockCount;
				nearest->label = label->label;
				mapLabelList[label->pos.y][label->pos.x] = label->label;
			}

		}
	}
}

bool FindShortestPath() {
	auto intPos = npc.trans.GetIntPos();

	std::queue<MapLabel> mapQueue;
	mapQueue.push({ intPos,1 });

	mapLabelList[intPos.y][intPos.x] = 1;

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
	if ((mapType == MAP_BLOCK || mapType == MAP_GOAL) && mapLabelList[label->pos.y][label->pos.x] == 0) {
		mapQueue->push(*label);

		mapLabelList[label->pos.y][label->pos.x] = label->label;
	}

}