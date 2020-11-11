// --------------------------------------------------------------
//							[trans.cpp]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include "trans.h"


void InitTrans(TRANS* trans,D3DXVECTOR2 initPos) {
	trans->pos = initPos;
	trans->intPos = {(int)initPos.x,(int)initPos.y};
	trans->intLastPos = trans->intPos;
}
void InitTrans(TRANS* trans, INTVECTOR2 initPos) {
	trans->pos.x = initPos.x;
	trans->pos.y = initPos.y;
	trans->intPos = initPos;
	trans->intLastPos = trans->intPos;
}

void UpdateTrans(TRANS* trans) {
	// 更新した位置を保管
	INTVECTOR2 compare;
	compare.x = (int)trans->pos.x;
	compare.y = (int)trans->pos.y;

	// 更新した位置と元の位置を比較
	if (compare == trans->intPos) {
		return;
	}

	trans->intLastPos = trans->intPos;

	// intPosに現在の位置を入れる
	trans->intPos = compare;
}
void UpdateTransX(TRANS* trans) {
	// 更新した位置を保管
	int x = (int)trans->pos.x;

	// 更新した位置と元の位置を比較
	if (x == trans->intPos.x) {
		return;
	}

	trans->intLastPos.x = trans->intPos.x;

	// intPosに現在の位置を入れる
	trans->intPos.x = x;
}
void UpdateTransY(TRANS* trans){
	// 更新した位置を保管
	int y = (int)trans->pos.y;

	// 更新した位置と元の位置を比較
	if (y == trans->intPos.y) {
		return;
	}

	trans->intLastPos.y = trans->intPos.y;

	// intPosに現在の位置を入れる
	trans->intPos.y = y;
}