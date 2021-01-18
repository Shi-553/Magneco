#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "myd3d.h"



void InitTexture();

void UninitTexture();



#define TEXTURE_MAX 1024
#define TEXTURE_FILENAME_MAX 256
#define TEXTURE_INVALID_ID -1

//テクスチャファイルの読み込み予約
//引数：ファイル名
//戻り値：最大管理数を超えていたらINVALID_TEXTURE_ID
int ReserveTextureLoadFile(const char* fileName);


//予約されたテクスチャファイルの読み込み
//戻り値：読み込めなかったファイル数
//---------デバイスの取得に失敗すると -1
int LoadTexture();


//テクスチャを選択して解放
//引数1：解放したいテクスチャID
void ReleaseTexture(int textureIds);


//テクスチャを選択して解放
//引数1：解放したいテクスチャIDの先頭アドレス
//引数2：解放したいテクスチャ数
void ReleaseTexture(int textureIds[], int count);


//テクスチャを全解放
void ReleaseAllTexture();

//参照カウントを１増やす
bool AddTextureReferenceCount(int textureId);

//テクスチャの大きさを取得
//引数：テクスチャID
//戻り値：テクスチャサイズ
D3DXVECTOR2* GetTextureSize(int textureId);


//テクスチャインターフェースポインタの取得
//引数：テクスチャID
//戻り値：テクスチャインターフェースポインタ
//---------ダメならNULL
LPDIRECT3DTEXTURE9 GetTexture(int textureId);

#endif // !_TEXTURE_H_
