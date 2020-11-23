
#include "highlight.h"
#include "config.h"
#include "texture.h"
#include "sprite.h"

struct Vertex2D{
	D3DXVECTOR4 pos;
};
#define FVF_VERTEX2D (D3DFVF_XYZRHW)

#define PI (3.141519)


void DrawCircle(D3DXVECTOR2 pos, float size, int count);

void SetBackStencilBufferDraw();
void SetBackStencilBufferMask();
void ResetBackStencilBufferDraw();
void ResetBackStencilBufferMask();



void DrawHighlight(D3DXVECTOR2 pos,float size,int count) {
	SetBackStencilBufferMask();

	DrawCircle(pos, size, count);

	ResetBackStencilBufferMask();
}

void DrawFrontBlack(D3DXCOLOR color) {
	SetBackStencilBufferDraw();
	SetSpriteColor(color);
	DrawSprite(NULL, {0,0 }, 100, { SCREEN_WIDTH,SCREEN_HEIGHT });
	SetSpriteColor(D3DCOLOR_RGBA(255,255,255,255));
	ResetBackStencilBufferDraw();
}


static DWORD g_dwCurZTest = 0;
static DWORD g_dwCurZFunc = 0;

void SetBackStencilBufferMask()
{
	auto device = GetD3DDevice();

	// Zバッファの設定を変更するので、現在の状態を保存しておく
	device->GetRenderState(D3DRS_ZENABLE, &g_dwCurZTest);
	device->GetRenderState(D3DRS_ZFUNC, &g_dwCurZFunc);

	// Zバッファに書き込まないようにします
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);


	device->SetRenderState(D3DRS_STENCILENABLE, true);  // ステンシルバッファ有効
	device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // ステンシルテストは常に行う
	device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	device->SetRenderState(D3DRS_STENCILREF, 0x01);
	device->SetRenderState(D3DRS_STENCILMASK, 0xff);
}
void SetBackStencilBufferDraw()
{
	auto device = GetD3DDevice();
	device->SetRenderState(D3DRS_STENCILENABLE, true);
	device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
	device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	device->SetRenderState(D3DRS_STENCILREF, 0x01);
	device->SetRenderState(D3DRS_STENCILMASK, 0xff);

}

void ResetBackStencilBufferMask() {
	auto device = GetD3DDevice();
	device->SetRenderState(D3DRS_ZENABLE, g_dwCurZTest);
	device->SetRenderState(D3DRS_ZFUNC, g_dwCurZFunc);
}

void ResetBackStencilBufferDraw() {
	auto device = GetD3DDevice();
	device->SetRenderState(D3DRS_STENCILENABLE, false);
}

void DrawCircle(D3DXVECTOR2 pos, float size, int count) {

	auto d3dDevice = GetD3DDevice();

	//このテクスチャ使ってー
	d3dDevice->SetTexture(0, NULL);

	//かんたん描画(遅い)
	//デバイスのFVF設定
	d3dDevice->SetFVF(FVF_VERTEX2D);

//中心で１+角の数+重複させる分１
	int vertexCount = count + 2;
	//メモリに頂点データ作る
	Vertex2D* vertexs = new Vertex2D[vertexCount];

	vertexs[0].pos = D3DXVECTOR4(pos.x, pos.y, 0, 0);

	for (int i = 1; i < vertexCount; i++) {
		float rad = (i * PI * 2) / count;
		float px = pos.x + cosf(rad) * size;
		float py = pos.y + sinf(rad) * size;

		vertexs[i].pos = D3DXVECTOR4(px, py, 0, 0);
	}
	d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, count, vertexs, sizeof(Vertex2D));

	delete[] vertexs;
}