// --------------------------------------------------------------
//							[trans.cpp]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------
//
#include "trans.h"

void InitTrans(TRANS* trans, D3DXVECTOR2 pos) {
	trans->pos = pos;

	trans->intPos.x = (int)trans->pos.x;
	trans->intPos.y = (int)trans->pos.y;

	trans->intLastPos.x = trans->intPos.x;
	trans->intLastPos.y = trans->intPos.y;


}


void ChangeTrans(TRANS* trans, D3DXVECTOR2 pos) {
	trans->pos = pos;

	INTVECTOR2 compare;
	// 更新した位置を保管
	compare.x = (int)trans->pos.x;
	compare.y = (int)trans->pos.y;

	// 更新した位置と元の位置を比較
	if (compare.x == trans->intPos.x && compare.y == trans->intPos.y) {
		return;
	}

	trans->intLastPos.x = trans->intPos.x;
	trans->intLastPos.y = trans->intPos.y;

	// intPosに現在の位置を入れる
	trans->intPos.x = compare.x;
	trans->intPos.y = compare.y;

}

void MoveTrans(TRANS* trans, D3DXVECTOR2 addPos) {

	// 位置にaddPos分追加する
	trans->pos += addPos;

	INTVECTOR2 compare;
	// 更新した位置を保管
	compare.x = (int)trans->pos.x;
	compare.y = (int)trans->pos.y;

	// 更新した位置と元の位置を比較
	if (compare.x == trans->intPos.x && compare.y == trans->intPos.y) {
		return;
	}

	trans->intLastPos.x = trans->intPos.x;
	trans->intLastPos.y = trans->intPos.y;

	// intPosに現在の位置を入れる
	trans->intPos.x = compare.x;
	trans->intPos.y = compare.y;


}

void MoveTransX(TRANS* trans, float x) {
	INTVECTOR2 compare;

	// 位置にaddPos分追加する
	trans->pos.x += x;

	// 更新した位置を保管
	compare.x = (int)trans->pos.x;

	// 更新した位置と元の位置を比較
	if (compare.x == trans->intPos.x) {
		return;
	}

	trans->intLastPos.x = trans->intPos.x;

	// intPosに現在の位置を入れる
	trans->intPos.x = compare.x;


}

void MoveTransY(TRANS* trans, float y) {
	INTVECTOR2 compare;

	// 位置にaddPos分追加する
	trans->pos.y += y;

	// 更新した位置を保管
	compare.y = (int)trans->pos.y;

	// 更新した位置と元の位置を比較
	if (compare.y == trans->intPos.y) {
		return;
	}

	trans->intLastPos.y = trans->intPos.y;

	// intPosに現在の位置を入れる
	trans->intPos.y = compare.y;


}