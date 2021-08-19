#pragma once
#include "Tutorial.h"
#include "messeage.h"
#include "messeagebox.h"
#include "movie.h"

class TutorialAdvanced1 :public ITutorial {
public:

	void Init()override;
	void Uninit()override;
	void Draw()override;
	void Update()override;

	virtual int GetStageIndex()const override {
		return 2;
	}

private:

	enum TextureEnum {
		MESSAGE_BOX,
		BACKGROUND,
		MAX
	};

	int textureIds[MAX];

	int num = 0;
	Message* message;

	Movie* movie;

};