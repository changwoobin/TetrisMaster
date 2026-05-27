#include "Object.h"

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