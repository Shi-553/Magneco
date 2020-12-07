//

#include "texture.h"
#include "myd3d.h"
#include "debugPrintf.h"

#define M_PI 3.141519

typedef struct Vertex2D_tag {
	D3DXVECTOR4 pos;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}Vertex2D;

#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//FVF：フレキシブルバーテックスフォーマット
//RHW=1 だと座標変換済み頂点だと勘違いする


LPDIRECT3DVERTEXBUFFER9 vertexBuffer1 = NULL;

LPDIRECT3DVERTEXBUFFER9 vertexBuffer2 = NULL;
LPDIRECT3DINDEXBUFFER9 indexBuffer = NULL;


void SetSpritePos(Vertex2D* v, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size);
void Rotate(Vertex2D* v, D3DXVECTOR2 pos, D3DXVECTOR2 cPos, float degreeAngle);
void SetSpriteUV(Vertex2D* v, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, bool  flipLR );
void SetSpriteColor(Vertex2D* v, D3DCOLOR color);
void SetSpriteUV01(Vertex2D* v, bool  flipLR);
void MatrixTransform(Vertex2D* v, D3DXVECTOR2 pos, D3DXVECTOR2 cPos, D3DXMATRIX* matrix);



D3DCOLOR color;
void SetSpriteColor(D3DCOLOR s) {
	color = s;
}

void InitSprite() {
	color = D3DCOLOR_RGBA(255, 255, 255, 255);

	int error = LoadTexture();
	if (error != 0) {
		DebugPrintf("LoadTextureError %d\n", error);
	}

	LPDIRECT3DDEVICE9 device = GetD3DDevice();

	device->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &vertexBuffer1, NULL);
	device->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &vertexBuffer2, NULL);

	//                                                               ↓65536頂点まで
	device->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuffer, NULL);
}
void UninitSprite() {
	if (vertexBuffer1 != NULL) {
		vertexBuffer1->Release();
		vertexBuffer1 = NULL;
	}
	if (vertexBuffer2 != NULL) {
		vertexBuffer2->Release();
		vertexBuffer2 = NULL;
	}
	if (indexBuffer != NULL) {
		indexBuffer->Release();
		indexBuffer = NULL;
	}
	ReleaseAllTexture();
}

//UV 01 サイズ原寸大 DrawPrimitiveUPを使用
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, bool  flipLR ) {
	LPDIRECT3DDEVICE9 d3dDevice = GetD3DDevice();

	//このテクスチャ使ってー
	d3dDevice->SetTexture(0, GetTexture(textureId));


	//かんたん描画(遅い)
	//デバイスのFVF設定
	d3dDevice->SetFVF(FVF_VERTEX2D);

	//メモリに頂点データ作る
	Vertex2D vertexs[4];

	D3DXVECTOR2* size = GetTextureSize(textureId);
	if (size == NULL) {
		DebugPrintf("sizeError\n");
		return;
	}
	SetSpritePos(vertexs, pos, z, *size);

	SetSpriteColor(vertexs, color);

	SetSpriteUV01(vertexs, flipLR);


	d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexs, sizeof(Vertex2D));

}

// UV 01  DrawPrimitiveを使用
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, bool  flipLR ) {
	LPDIRECT3DDEVICE9 d3dDevice = GetD3DDevice();

	//このテクスチャ使ってー
	d3dDevice->SetTexture(0, GetTexture(textureId));


	//かんたん描画(遅い)
	//デバイスのFVF設定
	d3dDevice->SetFVF(FVF_VERTEX2D);


	//VRAMに頂点バッファ書き込む！
	Vertex2D* pV;
	vertexBuffer1->Lock(0, 0, (void**)&pV, 0);//offset 0、size 0で全ロックしてる


	SetSpritePos(pV, pos, z, size);

	SetSpriteColor(pV, color);

	SetSpriteUV01(pV, flipLR);

	vertexBuffer1->Unlock();

	//この頂点バッファ使って！
	d3dDevice->SetStreamSource(0, vertexBuffer1, 0, sizeof(Vertex2D));


	//描画
	d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


// DrawIndexedPrimitiveUPを使用
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, bool  flipLR ){
	LPDIRECT3DDEVICE9 d3dDevice = GetD3DDevice();

	//このテクスチャ使ってー
	d3dDevice->SetTexture(0, GetTexture(textureId));


	//かんたん描画(遅い)
	//デバイスのFVF設定
	d3dDevice->SetFVF(FVF_VERTEX2D);

	//メモリに頂点データ作る
	Vertex2D vertexs[4];

	SetSpritePos(vertexs,pos , z,size );

	SetSpriteColor(vertexs, color);

	D3DXVECTOR2* trueSize = GetTextureSize(textureId);
	if (trueSize == NULL) {
		DebugPrintf("sizeError\n");
		return;
	}

	SetSpriteUV(vertexs,* trueSize,tPos ,tSize, flipLR);


	//メモリにインデックスデータ作る
	//↓65536頂点まで
	unsigned short indexs[] = { 0,1,2,1,3,2 };

	//                                                                    ↓65536頂点まで
	d3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, indexs, D3DFMT_INDEX16, vertexs, sizeof(Vertex2D));
}


//回転もできる DrawIndexedPrimitiveを使用
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, D3DXVECTOR2 cPos, float rad, bool  flipLR) {
	LPDIRECT3DDEVICE9 d3dDevice = GetD3DDevice();

	//このテクスチャ使ってー
	d3dDevice->SetTexture(0, GetTexture(textureId));


	//かんたん描画(遅い)
	//デバイスのFVF設定
	d3dDevice->SetFVF(FVF_VERTEX2D);


	//VRAMに頂点バッファ書き込む！
	Vertex2D* pV;
	vertexBuffer1->Lock(0, 0, (void**)&pV, 0);//offset 0、size 0で全ロックしてる


	SetSpritePos(pV, pos, z, size);

	Rotate(pV,pos ,cPos , rad);

	SetSpriteColor(pV, color);

	D3DXVECTOR2* trueSize = GetTextureSize(textureId);
	if (trueSize == NULL) {
		DebugPrintf("sizeError\n");
		return;
	}

	SetSpriteUV(pV,*trueSize,tPos , tSize, flipLR);

	vertexBuffer1->Unlock();

	//この頂点バッファ使って！
	d3dDevice->SetStreamSource(0, vertexBuffer1, 0, sizeof(Vertex2D));


	//インデックスバッファ書き込む！
	WORD* pI;
	indexBuffer->Lock(0, 0, (void**)&pI, 0);//offset 0、size 0で全ロックしてる

// { 0,1,2,1,3,2 };
	pI[0] = 0;
	pI[1] = 1;
	pI[2] = 2;
	pI[3] = 1;
	pI[4] = 3;
	pI[5] = 2;
	indexBuffer->Unlock();

	//このインデックスバッファ使って！
	d3dDevice->SetIndices(indexBuffer);


	//ベースのインデックス？、最小インデックス、頂点数、最初のインデックス？、プリミティブ(この場合三角形)の数
	d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4,0,2);
}



//マトリックス DrawIndexedPrimitiveを使用
void DrawSprite(int textureId, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, D3DXVECTOR2 cPos, D3DXMATRIX* matrix,bool  flipLR) {
	LPDIRECT3DDEVICE9 d3dDevice = GetD3DDevice();

	//このテクスチャ使ってー
	d3dDevice->SetTexture(0, GetTexture(textureId));


	//かんたん描画(遅い)
	//デバイスのFVF設定
	d3dDevice->SetFVF(FVF_VERTEX2D);


	//VRAMに頂点バッファ書き込む！
	Vertex2D* pV;
	vertexBuffer1->Lock(0, 0, (void**)&pV, 0);//offset 0、size 0で全ロックしてる


	SetSpritePos(pV, pos, z, size);

	MatrixTransform(pV,pos ,cPos , matrix);

	SetSpriteColor(pV, color);

	D3DXVECTOR2* trueSize = GetTextureSize(textureId);
	if (trueSize == NULL) {
		DebugPrintf("sizeError\n");
		return;
	}

	SetSpriteUV(pV,*trueSize,tPos , tSize, flipLR);

	vertexBuffer1->Unlock();

	//この頂点バッファ使って！
	d3dDevice->SetStreamSource(0, vertexBuffer1, 0, sizeof(Vertex2D));


	//インデックスバッファ書き込む！
	WORD* pI;
	indexBuffer->Lock(0, 0, (void**)&pI, 0);//offset 0、size 0で全ロックしてる

// { 0,1,2,1,3,2 };
	pI[0] = 0;
	pI[1] = 1;
	pI[2] = 2;
	pI[3] = 1;
	pI[4] = 3;
	pI[5] = 2;
	indexBuffer->Unlock();

	//このインデックスバッファ使って！
	d3dDevice->SetIndices(indexBuffer);


	//ベースのインデックス？、最小インデックス、頂点数、最初のインデックス？、プリミティブ(この場合三角形)の数
	d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4,0,2);
}


void Rotate(Vertex2D* v, D3DXVECTOR2 pos, D3DXVECTOR2 cPos, float rad) {

	D3DXMATRIX mtxRotation, mtxTransToCenter, mtxResult;

	D3DXMatrixTranslation(&mtxTransToCenter, -pos.x - cPos.x, -pos.y - cPos.y, 0);

	D3DXMatrixRotationZ(&mtxRotation, rad-3.141519);

	mtxResult = mtxTransToCenter * mtxRotation * -mtxTransToCenter;


	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(&v[i].pos, &v[i].pos, &mtxResult);
	}
}


void MatrixTransform(Vertex2D* v, D3DXVECTOR2 pos, D3DXVECTOR2 cPos,D3DXMATRIX* matrix) {

	D3DXMATRIX  mtxTransToCenter, mtxRotation, mtxResult;

	D3DXMatrixTranslation(&mtxTransToCenter, -pos.x - cPos.x, -pos.y - cPos.y, 0);

	D3DXMatrixRotationZ(&mtxRotation,  3.141519);

	mtxResult = mtxTransToCenter * mtxRotation* *matrix * -mtxTransToCenter;


	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(&v[i].pos, &v[i].pos, &mtxResult);
	}
}


void SetSpritePos(Vertex2D* v, D3DXVECTOR2 pos, float z, D3DXVECTOR2 size) {

	v[0].pos = D3DXVECTOR4(pos.x - 0.5f, pos.y - 0.5f, z, 1.0f);

	v[1].pos = D3DXVECTOR4(pos.x + size.x - 0.5f, pos.y - 0.5f, z, 1.0f);

	v[2].pos = D3DXVECTOR4(pos.x - 0.5f, pos.y + size.y - 0.5f, z, 1.0f);

	v[3].pos = D3DXVECTOR4(pos.x + size.x - 0.5f, pos.y + size.y - 0.5f, z, 1.0f);

}
void SetSpriteColor(Vertex2D* v, D3DCOLOR color) {
	v[0].color = color;

	v[1].color = color;

	v[2].color = color;

	v[3].color = color;
}
void SetSpriteUV(Vertex2D* v, D3DXVECTOR2 trueSize, D3DXVECTOR2 tPos, D3DXVECTOR2 tSize, bool  flipLR ) {


	v[0].texcoord = D3DXVECTOR2(tPos.x / trueSize.x, tPos.y / trueSize.y);

	v[1].texcoord = D3DXVECTOR2((tPos.x + tSize.x) / trueSize.x, tPos.y / trueSize.y);

	v[2].texcoord = D3DXVECTOR2(tPos.x / trueSize.x, (tPos.y + tSize.y) / trueSize.y);

	v[3].texcoord = D3DXVECTOR2((tPos.x + tSize.x) / trueSize.x, (tPos.y + tSize.y) / trueSize.y);

	if (flipLR) {
		FLOAT l1=v[0].texcoord.x;
		v[0].texcoord.x =v[1].texcoord.x ;
		v[1].texcoord.x = l1;

		FLOAT l2=v[2].texcoord.x;
		v[2].texcoord.x = v[3].texcoord.x;
		v[3].texcoord.x = l2;
	}
}
void SetSpriteUV01(Vertex2D* v, bool  flipLR ) {

	v[0].texcoord = D3DXVECTOR2(0, 0);

	v[1].texcoord = D3DXVECTOR2(1, 0);

	v[2].texcoord = D3DXVECTOR2(0, 1);

	v[3].texcoord = D3DXVECTOR2(1, 1);

	if (flipLR) {
		FLOAT l1=v[0].texcoord.x;
		v[0].texcoord.x =v[1].texcoord.x ;
		v[1].texcoord.x = l1;

		FLOAT l2=v[2].texcoord.x;
		v[2].texcoord.x = v[3].texcoord.x;
		v[3].texcoord.x = l2;
	}
}