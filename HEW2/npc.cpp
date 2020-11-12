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


static int NPCTextureId = TEXTURE_INVALID_ID;
static NPC npc;

static int mapLabelList[MAPCHIP_HEIGHT][MAPCHIP_WIDTH];
std::stack<INTVECTOR2> nextPosQueue;

static INTVECTOR2 gBeaconPos;

int frame = 0;

void InitNPC() {
	NPCTextureId = ReserveTextureLoadFile("texture/npc.png");
	npc.speed = 1;
	npc.trans.Init(2, 7);
	gBeaconPos = npc.trans.GetIntPos();
	while (!nextPosQueue.empty()) {
		nextPosQueue.pop();
	}
}

void UninitNPC() {
	ReleaseTexture(NPCTextureId);
}

void UpdateNPC() {
	if (nextPosQueue.empty()) {
		frame = 0;
		return;
	}
	if (frame > 30) {
		npc.trans.pos = nextPosQueue.top().ToD3DXVECTOR2();
		npc.trans.UpdatePos();

		nextPosQueue.pop();

		frame = 0;

		if (GetMapType(npc.trans.pos) == MAP_GOAL) {
			GoNextScene(GameClearScene);
		}

		return;
	}
	frame++;
}

void DrawNPC() {
	DrawGameSprite(NPCTextureId, npc.trans.GetIntPos().ToD3DXVECTOR2(), 30);

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

		auto mapType = GetMapType(current.ToD3DXVECTOR2()+D3DXVECTOR2(1,0));
		if (mapLabelList[current.y][current.x + 1] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current.x++;
			continue;
		}
		mapType = GetMapType(current.ToD3DXVECTOR2() + D3DXVECTOR2(0, 1));
		if (mapLabelList[current.y + 1][current.x] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current.y++;
			continue;
		}
		mapType = GetMapType(current.ToD3DXVECTOR2() + D3DXVECTOR2(-1, 0));
		if (mapLabelList[current.y][current.x - 1] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current.x--;
			continue;
		}
		mapType = GetMapType(current.ToD3DXVECTOR2() + D3DXVECTOR2(0, -1));
		if (mapLabelList[current.y - 1][current.x] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			current.y--;
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
	auto mapType = GetMapType(label->pos.ToD3DXVECTOR2());
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
	auto mapType = GetMapType(label->pos.ToD3DXVECTOR2());
	if ((mapType == MAP_BLOCK || mapType == MAP_GOAL) && mapLabelList[label->pos.y][label->pos.x] == 0) {
		mapQueue->push(*label);

		mapLabelList[label->pos.y][label->pos.x] = label->label;
	}

}