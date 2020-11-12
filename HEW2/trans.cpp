// --------------------------------------------------------------
//							[trans.cpp]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include "trans.h"


void TRANS::UpdatePos() {
	// 更新した位置を保管
	INTVECTOR2 compare((int)pos.x, (int)pos.y);

	// 更新した位置と元の位置を比較
	if (compare == intPos) {
		return;
	}

	intLastPos = intPos;

	// intPosに現在の位置を入れる
	intPos = compare;
}
void TRANS::UpdateX() {
	// 更新した位置を保管
	int x = (int)pos.x;

	// 更新した位置と元の位置を比較
	if (x == intPos.x) {
		return;
	}

	intLastPos.x = intPos.x;

	// intPosに現在の位置を入れる
	intPos.x = x;
}
void TRANS::UpdateY(){
	// 更新した位置を保管
	int y = (int)pos.y;

	// 更新した位置と元の位置を比較
	if (y == intPos.y) {
		return;
	}

	intLastPos.y = intPos.y;

	// intPosに現在の位置を入れる
	intPos.y = y;
}