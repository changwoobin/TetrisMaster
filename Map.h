#pragma once
#include "Object.h"
#include <iostream>
using namespace std;

enum {
    BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE, DARK_RED, DARK_VIOLET,
    DARK_YELLOW, GRAY, DARK_GRAY, BLUE, GREEN, SKY_BLUE, RED, VIOLET, YELLOW, WHITE
};

class Map : public Object {
private:
    int totalBlock[21][14];

public:
    Map();

    // 원본 show_total_block()
    void draw(int level); // level에 따라 테두리 색을 바꾸어서 level을 인자로 받음
};


Map::Map() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13) {
                totalBlock[i][j] = 1;
            }
            else {
                totalBlock[i][j] = 0;
            }
        }
    }

    for (int j = 0; j < 14; j++) {
        totalBlock[20][j] = 1;
    }
}


void Map::draw(int level) {
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 14; j++) {

            if (j == 0 || j == 13 || i == 20) {
                changeColor((level % 6) + 1);   // 원본의 레벨별 테두리 색
            }
            else {
                changeColor(DARK_GRAY);         // 원본의 내부 칸 색
            }

            moveCursor((j * 2) + 5/*ab_x*/, i + 1/*ab_y*/);

            if (totalBlock[i][j] == 1) {
                cout << "■";
            }
            else {
                cout << "  ";
            }
        }
    }

    moveCursor(77, 23);
}