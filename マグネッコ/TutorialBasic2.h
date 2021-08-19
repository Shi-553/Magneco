
#include "Tutorial.h"
#include "messeage.h"
#include "messeagebox.h"
#include "movie.h"

class TutorialBasic2 :public ITutorial {
public:

	void Init()override;
	void Uninit()override;
	void Draw()override;
	void Update()override;

	virtual int GetStageIndex()const override {
		return 1;
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