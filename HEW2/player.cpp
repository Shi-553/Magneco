#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "flyingObject.h"
#include "myd3d.h"
#include <math.h>
#include <list>
#include "player.h"
#include "game.h"

typedef struct Player_tag{
	D3DXVECTOR2 position;
	D3DXVECTOR2 direction;
	std::list < FlyingObject > flyingObject;
}Player;

static int TextureId = TEXTURE_INVALID_ID;


void InitPlayer(){

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

}

void MoveDownPlayer(){

}

void MoveLeftPlayer(){

}

void MoveRightPlayer(){

}

void BlockDecision(FlyingObject flyngobject) {

}