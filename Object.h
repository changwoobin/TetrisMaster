#pragma once
#include <Windows.h>

class Object {
protected:
    // 원본 init()에서 사용한 화면 기준점
    // ab_x = 5, ab_y = 1;
    int abx;
    int aby;

public:
    Object();

    // 원본 gotoxy(int x, int y)
    void moveCursor(int x, int y);

    // 원본 SetColor(int color)
    void changeColor(int color);
};

