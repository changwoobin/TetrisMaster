#include "Map.h"

// 생성자로 절대 좌표 설정
Map::Map(int abx, int aby) : Object(abx, aby) {
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
                changeColor((level % 6) + 1);
            }
            else {
                changeColor(DARK_GRAY);
            }

            // 하드코딩 제거 후 절대 좌표 기준 수식 적용
            moveCursor((j * 2) + abx, i + aby);

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

void Map::setMap(int row, int col, int data) {
    totalBlock[row][col] = data;
}

int Map::operator()(int x, int y)
{
    return totalBlock[y][x];
}