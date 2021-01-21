//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,		    // 通常BGM(お船でゆらゆら)
	SOUND_LABEL_BGM001,		    // 通常BGM(小竜の背に乗って)
	SOUND_LABEL_BGM002,		    // ステージセレクト画面で流れるBGM(律儀なご説明)
	SOUND_LABEL_BGM003,		    // ゲームクリア時のBGM(小熊のポルカ)
	SOUND_LABEL_BGM004,		    // ゲームクリア時のBGM(みんなで山登り)
	SOUND_LABEL_BGM005,			// ゲームオーバー時のBGM(たなびく黄昏)
	SOUND_LABEL_BGM006,			// ゲームオーバー時のBGM(過去への旅路)
	SOUND_LABEL_BGM007,			// ボス戦BGM(決着をつけるぞ)
	SOUND_LABEL_BGM008,			// ボス戦BGM(逃げろ逃げろ逃げろ)
	SOUND_LABEL_BGM009,			// ボス戦BGM(予期せぬ事態)
	SOUND_LABEL_SE_COMPLETE,	// マグネッコがルートを作成しきった時のSE
	SOUND_LABEL_SE_DECITION,	// 決定したときのSE
	SOUND_LABEL_SE_EXPLOSION,	// 敵が爆発したときのSE
	SOUND_LABEL_SE_PURGE,		// マグネッコが所持ブロックをパージしたときのSE
	SOUND_LEBEL_SE_STAN,		// マグネッコスタン時のSE
	SOUND_LABEL_SE_STICK,		// マグネッコがブロックをくっつけた時のSE
	SOUND_LABEL_SE_UFO,			// UFOが漂うSE
	SOUND_LAGEL_SE_UFO_BEAM,	// UFOがローズをさらっているときのSE
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
