// テクスチャ管理 2020/6/26 柴田晴彦
#define _CRT_SECURE_NO_WARNINGS

#include "texture.h"
#include <string.h>
#include "debugPrintf.h"



typedef struct {
	LPDIRECT3DTEXTURE9 texture;
	char filename[TEXTURE_FILENAME_MAX];
	D3DXVECTOR2* size;
	int referenceCount;
}Texture;

void InitTextureStruct(Texture* t);

// Texture構造体の初期化
void InitTextureStruct(Texture* t) {
	if (t->texture != NULL) {
		t->texture->Release();
	}
	t->texture = NULL;

	if (t->size != NULL) {
		free(t->size);
	}
	t->size = NULL;

	t->filename[0] = '\0';
	t->referenceCount = 0;
}

static Texture textures[TEXTURE_MAX];

// texture.cppの初期化
void InitTexture() {
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		InitTextureStruct(&textures[i]);
	}
}
void UninitTexture() {
}


//テクスチャファイルの読み込み予約
//引数：ファイル名
//戻り値：最大管理数を超えていたらINVALID_TEXTURE_ID
int ReserveTextureLoadFile(const char* filename) {

	int freeIndex = -1;

	//既に予約されている場合予約しない
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (textures[i].referenceCount == 0) {
			//部屋が空いている
			if (freeIndex == -1) {
				freeIndex = i;
			}
			continue;
		}

		//ハッシュを作ると早くなる
		if (strcmp(filename, textures[i].filename) == 0) {
			textures[i].referenceCount++;

			//既に予約されている
			return i;
		}
	}

	if (freeIndex == -1) {
		return TEXTURE_INVALID_ID;
	}

	strcpy(textures[freeIndex].filename, filename);

	textures[freeIndex].referenceCount = 1;

	return freeIndex;
}


//予約されたテクスチャファイルの読み込み
//戻り値：読み込めなかったファイル数
//---------デバイスの取得に失敗すると -1
int LoadTexture() {
	LPDIRECT3DDEVICE9 device = GetD3DDevice();
	if (device == NULL) {
		return -1;
	}

	int errorCount = 0;

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (textures[i].filename[0] == '\0') {
			//予約されていない
			continue;
		}
		if (textures[i].texture != NULL) {
			//既に読み込まれている
			continue;
		}
		//テクスチャサイズを取得
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(textures[i].filename, &info);
		textures[i].size = (D3DXVECTOR2*)malloc(sizeof(D3DXVECTOR2));
		if (textures[i].size == NULL) {
			DebugPrintf("サイズ取得失敗 Index: %d ,Filename: %s\n", i, textures[i].filename);
			errorCount++;
			continue;
		}
		textures[i].size->x = info.Width;
		textures[i].size->y = info.Height;

		//動的にメモリを確保してそこに読み込んで、そのメモリからDirectXのテクスチャにすると効率がいい

		HRESULT hr = D3DXCreateTextureFromFileEx(device,
			textures[i].filename, 
			info.Width,info.Height, 
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, 
			D3DX_DEFAULT, 
			D3DX_DEFAULT,
			0, 
			NULL,
			NULL,
			&textures[i].texture);
		//HRESULT hr = D3DXCreateTextureFromFileInMemory(device, textures[i].filename, &textures[i].texture);

		if (FAILED(hr)) {
			DebugPrintf("テクスチャ読み込み失敗 Index: %d ,Filename: %s\n", i, textures[i].filename);
			errorCount++;
		}
	}

	return errorCount;
}

//テクスチャを選択して解放
//引数1：解放したいテクスチャ数
void ReleaseTexture(int textureId) {
	if (textureId < 0 || TEXTURE_MAX < textureId) {
		return;
	}

	Texture* t = &textures[textureId];

	if (t->texture == NULL) {
		return;
	}

	t->referenceCount--;

	if (t->referenceCount != 0) {
		return;
	}

	InitTextureStruct(t);
}

//引数1：解放したいテクスチャIDの先頭アドレス
//引数2：解放したいテクスチャ数
void ReleaseTexture(int textureIds[], int count) {
	for (int i = 0; i < count; i++)
	{
		ReleaseTexture(textureIds[i]);

	}
}

//テクスチャを全解放
void ReleaseAllTexture() {
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		InitTextureStruct(&textures[i]);
	}

}


//テクスチャのサイズを取得
//引数：テクスチャID
//戻り値：テクスチャのサイズ
D3DXVECTOR2* GetTextureSize(int textureId) {
	if (textureId < 0 || TEXTURE_MAX < textureId) {
		return NULL;
	}

	return textures[textureId].size;
}

//テクスチャインターフェースポインタの取得
//引数：テクスチャID
//戻り値：テクスチャインターフェースポインタ
//---------ダメならNULL
LPDIRECT3DTEXTURE9 GetTexture(int textureId) {
	if (textureId < 0 || TEXTURE_MAX < textureId) {
		return NULL;
	}

	return textures[textureId].texture;
}