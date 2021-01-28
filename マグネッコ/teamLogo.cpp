#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "sceneManager.h"
#include "InputLogger.h"

#define TEAM_LOGO_TEXTURE_WIDTH  1024
#define TEAM_LOGO_TEXTURE_HEIGHT 512

#define TEAM_LOGO_MAX_FRAME 100.0
#define TEAM_LOGO_DESCRIPTION_START_FRAME 70.0


#define TEAM_LOGO_PRESS_O_WIDTH 128
#define TEAM_LOGO_PRESS_O_HEIGHT 32

static int backGroundTexture = TEXTURE_INVALID_ID;
static int teamLogoTexture = TEXTURE_INVALID_ID;
static int descriptionTexture = TEXTURE_INVALID_ID;
static int pressOTexture = TEXTURE_INVALID_ID;

static int frame = 0;
static int animeFrame = 0;

void InitTeamLogo()
{
	backGroundTexture = ReserveTextureLoadFile("texture/teamlogo/black.png");
	teamLogoTexture = ReserveTextureLoadFile("texture/teamlogo/rogo_siro_1024×512.png");
	descriptionTexture = ReserveTextureLoadFile("texture/teamlogo/sousa_setumei.png");
	pressOTexture = ReserveTextureLoadFile("texture/ui/press_o_128×32.png");

	LoadTexture();

	frame = 0;
	animeFrame = 0;
}

void UninitTeamLogo()
{
	ReleaseTexture(backGroundTexture);
	ReleaseTexture(teamLogoTexture);
	ReleaseTexture(descriptionTexture);
	ReleaseTexture(pressOTexture);
}

void DrawTeamLogo()
{
	if (frame < TEAM_LOGO_MAX_FRAME) {
		DrawSprite(backGroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
		DrawSprite(teamLogoTexture, { 128,104 }, 10, { TEAM_LOGO_TEXTURE_WIDTH,TEAM_LOGO_TEXTURE_HEIGHT }, { 0,0 }, { TEAM_LOGO_TEXTURE_WIDTH,TEAM_LOGO_TEXTURE_HEIGHT });
	}
	if (frame > TEAM_LOGO_DESCRIPTION_START_FRAME) {
		SetSpriteColor(D3DCOLOR_RGBA(255,255,255, (int)(255*((frame- TEAM_LOGO_DESCRIPTION_START_FRAME)/ (TEAM_LOGO_MAX_FRAME- TEAM_LOGO_DESCRIPTION_START_FRAME)))));
		DrawSprite(descriptionTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
		DrawSprite(pressOTexture, { 990,590 }, 10,D3DXVECTOR2(TEAM_LOGO_PRESS_O_WIDTH, TEAM_LOGO_PRESS_O_HEIGHT),
			D3DXVECTOR2(0,32*((int)( animeFrame /8.0)%8)),D3DXVECTOR2(TEAM_LOGO_PRESS_O_WIDTH, TEAM_LOGO_PRESS_O_HEIGHT));
	}
	SetSpriteColor(D3DCOLOR_RGBA(255, 255, 255, 255));	
	
}

void UpdateTeamLogo()
{
	animeFrame++;
	if (frame < TEAM_LOGO_MAX_FRAME) {
		frame++;

		if (frame < TEAM_LOGO_DESCRIPTION_START_FRAME &&ReleaseInputLogger(MYVK_ENTER)) {
			frame = TEAM_LOGO_DESCRIPTION_START_FRAME;
		}
	}else
	if (frame == TEAM_LOGO_MAX_FRAME) {
		if (ReleaseInputLogger(MYVK_ENTER)) {
			GoNextScene(GameStartScene);
		}
	}
}