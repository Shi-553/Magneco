// --------------------------------------------------------------
//							[trans.h]
// 
//							Author:Kanna Noda
//----------------------------------------------------------------
#pragma once

#include <d3dx9.h>

struct INTVECTOR2 {
	int x;
	int y;

	INTVECTOR2() {
	}
	INTVECTOR2(int x, int y) {
		this->x = x;
		this->y = y;
	}
	INTVECTOR2(D3DXVECTOR2 pos) {
		this->x = (int)pos.x;
		this->y = (int)pos.y;
	}

	static const INTVECTOR2 GetNone() {
		return INTVECTOR2(0, 0);
	}

	static const INTVECTOR2 GetUp(){
		return INTVECTOR2(0, -1);
	}

	static const INTVECTOR2 GetDown() {
		return INTVECTOR2(0, 1);
	}

	static const INTVECTOR2 GetRight() {
		return INTVECTOR2(1, 0);
	}

	static const INTVECTOR2 GetLeft() {
		return INTVECTOR2(-1, 0);
	}

	static const INTVECTOR2 GetLowreRightCorner() {
		return INTVECTOR2(1, 1);
	}

	static const INTVECTOR2 GetUpperRightCorner() {
		return INTVECTOR2(1, -1);
	}

	static const INTVECTOR2 GetLowreLeftCorner() {
		return INTVECTOR2(-1, 1);
	}

	static const INTVECTOR2 GetUpperLeftCorner() {
		return INTVECTOR2(-1, -1);
	}

	const D3DXVECTOR2 ToD3DXVECTOR2()const {
		return D3DXVECTOR2(x, y);
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


class TRANS {
public:
	D3DXVECTOR2 pos;

	INTVECTOR2 GetIntPos() {
		return intPos;
	}
	INTVECTOR2 GetIntLastPos() {
		return intLastPos;
	}


	TRANS() {
	}
	TRANS(D3DXVECTOR2 initPos) {
		pos = initPos;
		Init();
	}
	TRANS(float initX,float initY) {
		pos.x = initX;
		pos.y = initY;
		Init();
	}
	TRANS(INTVECTOR2 initPos) {
		pos.x = initPos.x;
		pos.y = initPos.y;
		Init();
	}
	void Init(D3DXVECTOR2 initPos) {
		pos = initPos;
		Init();
	}
	void Init(float initX,float initY) {
		pos.x = initX;
		pos.y = initY;
		Init();
	}
	void Init(INTVECTOR2 initPos) {
		pos.x = initPos.x;
		pos.y = initPos.y;
		Init();
	}

	void UpdatePos();

private:
	INTVECTOR2 intPos;
	INTVECTOR2 intLastPos;

	void Init() {
		intPos = { (int)pos.x,(int)pos.y };
		intLastPos = intPos;
	}
};

