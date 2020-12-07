#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

enum Scene
{
	NullScene=-1,
	GameStartScene,
	GameScene,
	GameClearScene,
	GameOverScene,
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
void GoNextScene(Scene scene,FadeMode mode=FADE_OUT);

#endif // !_SCENE_MANAGER
