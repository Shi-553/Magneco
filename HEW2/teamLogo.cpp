#include "config.h"
#include "texture.h"
#include "sprite.h"
#include "sceneManager.h"

#define TEAM_LOGO_TEXTURE_WIDTH  1024
#define TEAM_LOGO_TEXTURE_HEIGHT 512

static int backGroundTexture;
static int teamLogoTexture;

static int frame = 0;

void InitTeamLogo()
{
	backGroundTexture = ReserveTextureLoadFile("texture/teamlogo/white.png");
	teamLogoTexture = ReserveTextureLoadFile("texture/teamlogo/rogo_kuro_1024×512.png");

	LoadTexture();

	frame = 0;
}

void UninitTeamLogo()
{
	ReleaseTexture(backGroundTexture);
	ReleaseTexture(teamLogoTexture);
}

void DrawTeamLogo()
{
	DrawSprite(backGroundTexture, { 0,0 }, 10, { SCREEN_WIDTH,SCREEN_HEIGHT }, { 0,0 }, { SCREEN_WIDTH,SCREEN_HEIGHT });
	DrawSprite(teamLogoTexture, { 128,104 }, 10, { TEAM_LOGO_TEXTURE_WIDTH,TEAM_LOGO_TEXTURE_HEIGHT }, { 0,0 }, { TEAM_LOGO_TEXTURE_WIDTH,TEAM_LOGO_TEXTURE_HEIGHT });
}

void UpdateTeamLogo()
{
	frame++;
	if (frame == 60) {
		GoNextScene(GameStartScene);
	}
}