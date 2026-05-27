#include "Map.h"

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

void Map::setMap(int row, int col, int data) {
    totalBlock[row][col] = data;
}

void Map::flip()
{
    int flipBlock[21][14];

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13) {
                flipBlock[i][j] = 1;
            }
            else {
                if (i == 0) {
                    flipBlock[i][j] = 0;
                }
                else {
                    flipBlock[i][j] = totalBlock[20 - i][j];
                }
            }
        }
    }

    for (int j = 0; j < 14; j++) {
        flipBlock[20][j] = 1;
    }

	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 14; j++) {
			totalBlock[i][j] = flipBlock[i][j];
		}
	}
}

bool Map::stepGravity() {
    int emptyline = 20;

    for (int i = 19; i >= 1; i--) {
        bool empty = true;
        for (int j = 1; j < 13; j++) {
            if (totalBlock[i][j] != 0) {
                empty = false;
                break;
            }
        }
        
        if (empty) {
            emptyline = i;
            break;
        }
    }


    if (emptyline == 20) {
        return false;
    }

    bool hasBlockAbove = false;
    for (int i = emptyline - 1; i >= 1; i--) {
        for (int j = 1; j < 13; j++) {
            if (totalBlock[i][j] != 0) {
                hasBlockAbove = true;
                break;
            }
        }
    }

    if (!hasBlockAbove) {
        return false;
    }
    
    for (int i = emptyline; i >= 1; i--) {
        for (int j = 1; j < 13; j++) {
            totalBlock[i][j] = totalBlock[i - 1][j];
        }
    }
    for (int j = 1; j < 13; j++) {
        totalBlock[0][j] = 0;
    }

    return true;
}

void Map::insertLine(int pos, int line[])
{
    for (int i = 1; i < pos; i++) {
        for (int j = 1; j < 13; j++) {
            totalBlock[i - 1][j] = totalBlock[i][j];
        }
    }

    for (int i = 1; i < 13; i++) {
        totalBlock[pos][i] = line[i - 1];
    }
}

int Map::operator()(int x, int y)
{
    return totalBlock[y][x];
}
