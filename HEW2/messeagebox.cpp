#include "texture.h"
#include "config.h"
#include "sprite.h"
#include "InputLogger.h"


#define MESSEAGE_BOX_WIDTH 860
#define MESSEAGE_BOX_HEIGHT 260


static int backgroundTexture;

void InitMesseageBox(){

	backgroundTexture = ReserveTextureLoadFile("texture/Textbox_Test.png");

	LoadTexture();
}

void UninitMesseageBox(){

	ReleaseTexture(backgroundTexture);
}

void DrawMesseageBox(){
	DrawSprite(backgroundTexture, { 200, SCREEN_HEIGHT / 2 + 50}, 10, { MESSEAGE_BOX_WIDTH, MESSEAGE_BOX_HEIGHT });
}

void UpdateMesseageBox(){
	/*if (TriggerInputLogger(MYVK_ENTER)) {
	
	}*/

}


