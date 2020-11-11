// --------------------------------------------------------------
//							[trans.h]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include <d3dx9.h>

typedef struct INTVECTOR2 {
	int x;
	int y;
};

typedef struct TRANS {
	D3DXVECTOR2 pos;
	INTVECTOR2 intPos;
	INTVECTOR2 intLastPos;
};
void InitTrans(TRANS* trans, D3DXVECTOR2 pos);
void ChangeTrans(TRANS* trans, D3DXVECTOR2 pos);
void MoveTrans(TRANS* trans, D3DXVECTOR2 addPos);
void MoveTransX(TRANS* trans, float x);
void MoveTransY(TRANS* trans, float y);

