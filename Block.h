#pragma once
#include "Object.h"
#include "Color.h"
#include <iostream>
#include <vector>

using namespace std;

class Block : public Object
{	
private:
	// 원본 전역변수 block_x, block_y, block_angle, block_shape
	int shape;
	int x, y;
	int angle;

public:
	// 원본 char block[7][4][4][4] = [shape][angle][row][col]
	static const int blocks[7][4][4][4];

	Block() = delete;
	Block(int shape);

	// 원본 block_start()
	void set(int s, int startX, int startY, int startAngle = 0);

	// 원본 rotate_block()
	void rotate();

	// 원본 show_cur_block(int shape, int angle, int x, int y)
	void show();

	// 원본 erase_cur_block(int shape, int angle, int x, int y)
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

	// 원본 block[shape][angle][j][i] 확인용
	int cell(int r, int c) const;
};

