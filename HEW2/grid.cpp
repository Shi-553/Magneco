#include "myd3d.h"
#include "gameSrite.h"
#include <malloc.h>

typedef struct Vertex2D_tag {
	D3DXVECTOR4 pos;
	D3DCOLOR color;
}Vertex2D;

#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


static Vertex2D* g_pVertex = NULL;
static int g_VertexCount = 0;
static int g_LineCount = 0;


void Grid_Initialize(float size, int count, D3DCOLOR color)
{
	count -= 2;
	g_LineCount = (count + 1) * 2;
	g_VertexCount = g_LineCount * 2;

	g_pVertex = (Vertex2D*)malloc(g_VertexCount * sizeof(Vertex2D));

	float length = size * count;
	float start = 0.0f;
	float end = length * 1.0f;
	int n = (count + 1) * 2;


	for (int i = 0; i < count + 1; i++) {
		g_pVertex[i * 2].pos = D3DXVECTOR4(start + size * i + GAME_SPRITE_ZERO_X + size,  start + GAME_SPRITE_ZERO_Y + size, 1.0f, 1.0f);
		g_pVertex[i * 2 + 1].pos = D3DXVECTOR4(start + size * i + GAME_SPRITE_ZERO_X + size,  end + GAME_SPRITE_ZERO_Y + size, 1.0f, 1.0f);
		g_pVertex[n + i * 2].pos = D3DXVECTOR4(start + GAME_SPRITE_ZERO_X + size,  start + size * i + GAME_SPRITE_ZERO_Y + size, 1.0f, 1.0f);
		g_pVertex[n + i * 2 + 1].pos = D3DXVECTOR4(end + GAME_SPRITE_ZERO_X + size,  start + size * i + GAME_SPRITE_ZERO_Y + size, 1.0f, 1.0f);
	}

	for (int i = 0; i < g_VertexCount; i++) {
		g_pVertex[i].color = color;
	}
}

void Grid_Finalize(void)
{
	free(g_pVertex);
}

void Grid_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	// デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, NULL);

	// ポリゴンを描画する
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, g_LineCount, g_pVertex, sizeof(Vertex2D));
}