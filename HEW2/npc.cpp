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

typedef struct MapLabel {
	int x, y, label, notBlockCount;
};

D3DXVECTOR2 FindNearestBlock();
void FourDirFindNearestBlock(std::deque<MapLabel>* mapQueue, MapLabel* label, MapLabel* nearest);

bool FindShortestPath();
void FourDir(std::queue<MapLabel>* mapQueue, MapLabel* label);


static int NPCTextureId = TEXTURE_INVALID_ID;
static NPC npc;

static int mapLabelList[MAPCHIP_HEIGHT][MAPCHIP_WIDTH];
std::stack<D3DXVECTOR2> nextPosQueue;

static D3DXVECTOR2 gBeaconPos;

int frame = 0;

void InitNPC() {
	NPCTextureId = ReserveTextureLoadFile("texture/npc.png");
	npc.speed = 1;
	npc.pos = D3DXVECTOR2(2, 7);
	gBeaconPos = npc.pos;
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
		npc.pos = nextPosQueue.top();

		nextPosQueue.pop();

		frame = 0;
		return;
	}
	frame++;
}

void DrawNPC() {
	D3DXVECTOR2 intposition;
	intposition = D3DXVECTOR2((int)npc.pos.x, (int)npc.pos.y);
	DrawGameSprite(NPCTextureId, intposition, 30);

}
void UpdateNPCShortestPath(D3DXVECTOR2 beaconPos) {
	gBeaconPos = beaconPos;
	UpdateNPCShortestPath();
}

void UpdateNPCShortestPath() {
	if (gBeaconPos == npc.pos) {
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

	int currentX;
	int currentY;
	if (FindShortestPath()) {
		currentX = (int)gBeaconPos.x;
		currentY = (int)gBeaconPos.y;

	}
	else {
		auto nearest = FindNearestBlock();
		currentX = (int)nearest.x;
		currentY = (int)nearest.y;
	}
	while (true) {
		if ((int)npc.pos.x == currentX && (int)npc.pos.y == currentY) {
			break;
		}
		nextPosQueue.push(D3DXVECTOR2(currentX, currentY));

		int label = mapLabelList[currentY][currentX];

		auto mapType = GetMapType({ (float)currentX + 1 ,(float)currentY });
		if (mapLabelList[currentY][currentX + 1] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			currentX++;
			continue;
		}
		mapType = GetMapType({ (float)currentX  ,(float)currentY + 1 });
		if (mapLabelList[currentY + 1][currentX] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			currentY++;
			continue;
		}
		mapType = GetMapType({ (float)currentX - 1 ,(float)currentY });
		if (mapLabelList[currentY][currentX - 1] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			currentX--;
			continue;
		}
		mapType = GetMapType({ (float)currentX  ,(float)currentY - 1 });
		if (mapLabelList[currentY - 1][currentX] == label - 1 && (mapType == MAP_BLOCK || mapType == MAP_GOAL)) {
			currentY--;
			continue;
		}
	}
}
D3DXVECTOR2 FindNearestBlock() {
	auto intBeaconPosX = (int)gBeaconPos.x;
	auto intBeaconPosY = (int)gBeaconPos.y;

	std::deque<MapLabel> mapQueue;
	mapQueue.push_back({ intBeaconPosX, intBeaconPosY,-1,0 });

	mapLabelList[intBeaconPosY][intBeaconPosX] = -1;

	MapLabel nearest = {
		npc.pos.x,
		npc.pos.y,
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
		right.x++;
		FourDirFindNearestBlock(&mapQueue, &right, &nearest);

		auto bottom = mapLabel;
		bottom.y++;
		FourDirFindNearestBlock(&mapQueue, &bottom, &nearest);

		auto left = mapLabel;
		left.x--;
		FourDirFindNearestBlock(&mapQueue, &left, &nearest);

		auto top = mapLabel;
		top.y--;
		FourDirFindNearestBlock(&mapQueue, &top, &nearest);

	}

	return { (float)nearest.x,(float)nearest.y };
}
void FourDirFindNearestBlock(std::deque<MapLabel>* mapQueue, MapLabel* label, MapLabel* nearest) {
	auto mapType = GetMapType({ (float)label->x,(float)label->y });
	//そこが到達可能なとき
	if (mapLabelList[label->y][label->x] > 0) {
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

		if (mapLabelList[label->y][label->x] == 0) {
			mapQueue->push_back(*label);

			mapLabelList[label->y][label->x] = label->label;
		}
		else {
			for (auto itr = mapQueue->begin(); itr != mapQueue->end(); itr++) {
				if (itr->x == label->x && itr->y == label->y &&
					(itr->notBlockCount > label->notBlockCount || (itr->notBlockCount == label->notBlockCount && itr->label < label->label))) {
					itr->notBlockCount = label->notBlockCount;
					itr->label = label->label;
					mapLabelList[label->y][label->x] = label->label;
				}
			}
			if (nearest->x == label->x && nearest->y == label->y &&
				(nearest->notBlockCount > label->notBlockCount ||(nearest->notBlockCount == label->notBlockCount && nearest->label < label->label))) {
				nearest->notBlockCount = label->notBlockCount;
				nearest->label = label->label;
				mapLabelList[label->y][label->x] = label->label;
			}

		}
	}
}

bool FindShortestPath() {
	auto intNPCPosX = (int)npc.pos.x;
	auto intNPCPosY = (int)npc.pos.y;

	std::queue<MapLabel> mapQueue;
	mapQueue.push({ intNPCPosX, intNPCPosY,1 });

	mapLabelList[intNPCPosY][intNPCPosX] = 1;

	while (!mapQueue.empty()) {
		auto mapLabel = mapQueue.front();
		mapQueue.pop();

		mapLabel.label++;

		auto right = mapLabel;
		right.x++;
		FourDir(&mapQueue, &right);
		if (right.x == gBeaconPos.x && right.y == gBeaconPos.y) {
			return true;
		}

		auto bottom = mapLabel;
		bottom.y++;
		FourDir(&mapQueue, &bottom);
		if (bottom.x == gBeaconPos.x && bottom.y == gBeaconPos.y) {
			return true;
		}

		auto left = mapLabel;
		left.x--;
		FourDir(&mapQueue, &left);
		if (left.x == gBeaconPos.x && right.y == gBeaconPos.y) {
			return true;
		}

		auto top = mapLabel;
		top.y--;
		FourDir(&mapQueue, &top);
		if (top.x == gBeaconPos.x && top.y == gBeaconPos.y) {
			return true;
		}

	}

	return false;
}
void FourDir(std::queue<MapLabel>* mapQueue, MapLabel* label) {
	auto mapType = GetMapType({ (float)label->x,(float)label->y });
	if ((mapType == MAP_BLOCK || mapType == MAP_GOAL) && mapLabelList[label->y][label->x] == 0) {
		mapQueue->push(*label);

		mapLabelList[label->y][label->x] = label->label;
	}

}