// --------------------------------------------------------------
//							[trans.h]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------

#include <d3dx9.h>

typedef struct INTVECTOR2 {
	int x;
	int y;

	INTVECTOR2() {
		x = 0;
		y = 0;
	}
	INTVECTOR2(int x, int y) {
		this->x = x;
		this->y = y;
	}

	bool operator==(const INTVECTOR2& b)const {
		return x == b.x && y == b.y;
	}
	INTVECTOR2& operator = (const INTVECTOR2& b) {
		x = b.x;
		y = b.y;
		return *this;
	}
	const INTVECTOR2 operator + (const INTVECTOR2& b) const
	{
		return { x + b.x, y + b.y };
	}
	const INTVECTOR2 operator - (const INTVECTOR2& b) const
	{
		return { x - b.x, y - b.y };
	}
	const INTVECTOR2 operator * (const INTVECTOR2& b) const
	{
		return { x * b.x, y * b.y };
	}
	const INTVECTOR2 operator / (const INTVECTOR2& b) const
	{
		return { x / b.x, y / b.y };
	}
	INTVECTOR2& operator += (const INTVECTOR2& b)
	{
		x += b.x;
		y += b.y; 
		return *this;
	}
	INTVECTOR2& operator -= (const INTVECTOR2& b)
	{
		x -= b.x;
		y -= b.y; 
		return *this;
	}
	INTVECTOR2& operator *= (const INTVECTOR2& b)
	{
		x *= b.x;
		y *= b.y; 
		return *this;
	}
	INTVECTOR2& operator /= (const INTVECTOR2& b)
	{
		x /= b.x;
		y /= b.y; 
		return *this;
	}
};


typedef struct TRANS {
	D3DXVECTOR2 pos;
	INTVECTOR2 intPos;
	INTVECTOR2 intLastPos;
};

void InitTrans(TRANS* trans, D3DXVECTOR2 initPos);
void InitTrans(TRANS* trans, INTVECTOR2 initPos);
void UpdateTrans(TRANS* trans);
void UpdateTransX(TRANS* trans);
void UpdateTransY(TRANS* trans);
