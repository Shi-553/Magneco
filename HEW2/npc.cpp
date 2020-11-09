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
	int x, y, label;
};

D3DXVECTOR2 FindNearestBlock();
bool FourDirFindNearestBlock(std::queue<MapLabel>* mapQueue, MapLabel* label);

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
		if (mapLabelList[currentY][currentX + 1] == label - 1 &&( mapType ==MAP_BLOCK|| mapType ==MAP_GOAL)) {
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

	std::queue<MapLabel> mapQueue;
	mapQueue.push({ intBeaconPosX, intBeaconPosY,-1 });

	mapLabelList[intBeaconPosY][intBeaconPosX] = -1;

	D3DXVECTOR2 nearestPos = npc.pos;
	int nearestLabel=-10000;

	while (!mapQueue.empty()) {
		auto mapLabel = mapQueue.front();
		mapQueue.pop();

		mapLabel.label--;

		auto right = mapLabel;
		right.x++;
		if (FourDirFindNearestBlock(&mapQueue, &right)) {
			if (right.label > nearestLabel) {
				nearestLabel = right.label;
				nearestPos.x = right.x;
				nearestPos.y = right.y;
			}
		}

		auto bottom = mapLabel;
		bottom.y++;
		if (FourDirFindNearestBlock(&mapQueue, &bottom)) {
			if (bottom.label > nearestLabel) {
				nearestLabel = bottom.label;
				nearestPos.x = bottom.x;
				nearestPos.y = bottom.y;
			}
		}

		auto left = mapLabel;
		left.x--;
		if (FourDirFindNearestBlock(&mapQueue, &left)) {
			if (left.label > nearestLabel) {
				nearestLabel = left.label;
				nearestPos.x = left.x;
				nearestPos.y = left.y;
			}
		}

		auto top = mapLabel;
		top.y--;
		if (FourDirFindNearestBlock(&mapQueue, &top)) {
			if (top.label > nearestLabel) {
				nearestLabel = top.label;
				nearestPos.x = top.x;
				nearestPos.y = top.y;
			}
		}

	}

	return nearestPos;
}
bool FourDirFindNearestBlock(std::queue<MapLabel>* mapQueue, MapLabel* label) {
	auto mapType = GetMapType({ (float)label->x,(float)label->y });
	if ((mapType == MAP_BLOCK || mapType == MAP_GOAL) && mapLabelList[label->y][label->x] > 0) {
		return true;
	}
	if (mapType == MAP_BLOCK_NONE && mapLabelList[label->y][label->x] == 0) {
		mapQueue->push(*label);

		mapLabelList[label->y][label->x] = label->label;
	}

	return false;
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