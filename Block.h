#pragma once
#include "Object.h"
#include "Color.h"
#include <iostream>
#include <vector>

using namespace std;

class Block : public Object
{
private:
	int shape;
	int x, y;
	int angle;

public:
	static const int blocks[7][4][4][4];

	Block() = delete;
	// 블록 역시 생성될 때 위치할 구역의 절대 좌표를 설정
	Block(int shape, int abx = 5, int aby = 1);

	void set(int s, int startX, int startY, int startAngle = 0);
	void rotate();
	void show();
	void erase();
	void moveDown();
	void moveUp();
	void moveLeft();
	void moveRight();
	void setCord(int x, int y);

	int getX() const;
	int getY() const;
	int getAngle() const;
	int getShape() const;

	int cell(int r, int c) const;
};