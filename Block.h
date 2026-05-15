#pragma once
#include "Object.h"
#include <iostream>
using namespace std;

enum {
	BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE, DARK_RED, DARK_VIOLET,
	DARK_YELLOW, GRAY, DARK_GRAY, BLUE, GREEN, SKY_BLUE, RED, VIOLET, YELLOW, WHITE
};

class Block : public Object
{	
private:
	// 원본 전역변수 block_x, block_y, block_angle, block_shape
	int shape;
	int x, y;
	int angle;

public:
	// 원본 char block[7][4][4][4] = [shape][angle][row][col]
	static const int blocks[7][4][4][4] = {
		//막대모양
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,	1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,

		//네모모양
		1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,

		//'ㅓ' 모양
		0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,	1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,	0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,

		//'ㄱ'모양
		1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,	1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,	0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,

		//'ㄴ' 모양
		1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,	1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,	1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,

		//'Z' 모양
		1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,	1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,	0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,

		//'S' 모양
		0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,	0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0
	};

	Block();

	// 원본 block_start()
	void set(int s, int startX, int startY, int startAngle = 0);

	// 원본 rotate_block()
	void rotate();

	// 원본 show_cur_block(int shape, int angle, int x, int y)
	void show();

	// 원본 erase_cur_block(int shape, int angle, int x, int y)
	void erase();

	int getX() const;
	int getY() const;
	int getAngle() const;
	int getShape() const;

	// 원본 block[shape][angle][j][i] 확인용
	int cell(int r, int c) const;
};

Block::Block() {
	x = 5;      // 원본 block_start(): x = 5
	y = -4;     // 원본 block_start(): y = -4
	angle = 0;   // 원본 block_start(): angle = 0
	shape = 0;
}


void Block::set(int s, int startX, int startY, int startAngle) {
	shape = s;
	x = startX;
	y = startY;
	angle = startAngle;
}

void Block::rotate() {
	angle = (angle + 1) % 4;
}

void Block::show() {
    switch (shape) {
    case 0: changeColor(RED); break;
    case 1: changeColor(BLUE); break;
    case 2: changeColor(SKY_BLUE); break;
    case 3: changeColor(WHITE); break;
    case 4: changeColor(YELLOW); break;
    case 5: changeColor(VIOLET); break;
    case 6: changeColor(GREEN); break;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (j + y < 0)
                continue;

            if (block[shape][angle][j][i] == 1) {
                // 원본 좌표식:
                // gotoxy((i+x)*2+ab_x, j+y+ab_y)
                // ab_x = 5, ab_y = 1
                moveCursor((i + x) * 2 + 5/*ab_x*/, j + y + 1/*ab_y*/);
                cout << "■";
            }
        }
    }

    moveCursor(77, 23);
}


void Block::erase() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[shape][angle][j][i] == 1) {
                if (y + j < 0)
                    continue;

                moveCursor((i + x) * 2 + 5/*ab_x*/, j + y + 1/*ab_y*/);
                cout << "  ";
            }
        }
    }
}

int Block::getX() const {
    return x;
}

int Block::getY() const {
    return y;
}

int Block::getAngle() const {
    return angle;
}

int Block::getShape() const {
    return shape;
}

int Block::cell(int r, int c) const {
    return block[shape][angle][r][c];
}