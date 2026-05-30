#include "Block.h"

const int Block::blocks[7][4][4][4] = {
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

Block::Block(int shape, int abx, int aby)
	: Object(abx, aby), shape(shape), x(15), y(1), angle(0), is_Next(false), is_Hold(false)
{
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

            if (blocks[shape][angle][j][i] == 1) {
                // Object의 절대좌표(abx, aby) 사용
                moveCursor(i+x, j+y);
                cout << "■";
            }
        }
    }
    moveCursor(36, 22);
}


void Block::erase() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blocks[shape][angle][j][i] == 1) {
                if (y + j < 0)
                    continue;

                moveCursor(i + x, j + y);
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

void Block::moveDown() {
    y++;
}

void Block::moveUp() {
    y--;
}

void Block::moveLeft() {
    x--;
}

void Block::moveRight() {
    x++;
}

void Block::setCord(int x, int y) {
    this->x = x;
    this->y = y;
}

int Block::cell(int r, int c) const {
    return blocks[shape][angle][r][c];
}