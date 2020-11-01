#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "flyingObject.h"
#include "myd3d.h"
#include <list>
#include "player.h"
#include "game.h"

typedef struct Player_tag{
	D3DXVECTOR2 position;
	std::list < FlyingObject > flyingObjectList;
}Player;

static int TextureId = TEXTURE_INVALID_ID;
static Player player;

void InitPlayer(){
	player.position = D3DXVECTOR2(3, 3);
	player.flyingObjectList.clear();
}

void UninitPlayer(){

}

void UpdatePlayer(){

}

void DrawPlayer(){

}


void RotateLeftPlayer(){

}

void RotateRightPlayer(){

}

void MoveUpPlayer(){
	player.position.y += -1.0f;
}

void MoveDownPlayer(){
	player.position.y += 1.0f;
}

void MoveLeftPlayer(){
	player.position.x += -1.0f;
}

void MoveRightPlayer(){
	player.position.x += 1.0f;
}

void BlockDecision(FlyingObject flyngObject) {

}