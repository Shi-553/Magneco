#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

enum Scene
{
	NullScene=-1,
	TeamLogoScene,
	GameStartScene,
	GameScene,
	GameClearScene,
	GameOverScene,
	StageEditorScene,
	StageSelect,
	StaffRoleScene,
	MaxScene
};
enum FadeMode {
	FADE_NONE,
	FADE_IN,
	FADE_OUT
};

void InitSceneManager(Scene startScene);
void UpdateSceneManager();
void DrawSceneManager();
void UninitSceneManager();
void GoNextScene(Scene scene,FadeMode mode=FADE_OUT, bool isForce = false);

Scene GetCurrentScene();
#endif // !_SCENE_MANAGER
