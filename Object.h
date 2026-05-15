#pragma once
#include <Windows.h>

class Object {
private:
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

Object::Object() {
    abx = 5;
    aby = 1;
}

void Object::moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(hConsole, pos);
}

void Object::changeColor(int color) {
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
