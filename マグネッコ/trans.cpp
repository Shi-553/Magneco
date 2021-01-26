#include "trans.h"


void TRANS::UpdatePos() {
	// 更新した位置を保管
	INTVECTOR2 compare=INTVECTOR2(pos);

	// 更新した位置と元の位置を比較
	if (compare == intPos) {
		return;
	}

	intLastPos = intPos;

	// intPosに現在の位置を入れる
	intPos = compare;
}