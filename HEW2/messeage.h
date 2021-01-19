#pragma once

#include <d3dx9.h>
#include "trans.h"


class Message {

	D3DXVECTOR2 fontScale;
	D3DXVECTOR2 fontPos;
	D3DXVECTOR2 endPos;


	long fontMarginY;

	LPD3DXFONT font;
	INTVECTOR2 offset;
	LPD3DXSPRITE sprite;
	D3DCOLOR color;
	int format;

	void UpdateRect(RECT& rect,const char* str);

public:
	Message(D3DXVECTOR2 scale = D3DXVECTOR2(1, 1));
	~Message();
	//行ごとのマージン
	void SetMargin(long margin);

	void SetScale(D3DXVECTOR2 scale);

	void SetPos(D3DXVECTOR2 pos);
	void SetEndPos(D3DXVECTOR2 pos);

	//DT_から始まるやつ
	void SetFormat(int f);
	D3DXVECTOR2 GetOneSize();
	void ClearOffset();


	void SetColor(D3DCOLOR c);

	void Draw(const char* str, va_list argp);

	void Draw(const char* str, ...);


};