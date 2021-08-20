#include "Tutorial.h"
#include "messeage.h"
#include "messeagebox.h"
#include "movie.h"

class TutorialAdvanced2 :public ITutorial {
public:

	void Init()override;
	void Uninit()override;
	void Draw()override;
	void Update()override;

	virtual int GetStageIndex()const override {
		return 3;
	}

private:

	enum {
		MESSAGE_BOX,
		BACKGROUND,
		MAX
	};

	int TextureIds[MAX];

	int num = 0;
	Message* message;

	Movie* movie;

};