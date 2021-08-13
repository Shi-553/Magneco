#pragma once

class ITutorial {
public:
	virtual void Init()=0;
	virtual void Uninit()=0;
	virtual void Draw()=0;
	virtual void Update()=0;


	virtual int GetStageIndex()const = 0;

	bool isUpdate = true;
};

void InitTutorial();
void UpdateTutorial();
void DrawTutorial();
void UninitTutorial();

