#include "Object.h"

// 생성자에 절대 좌표 반영
Object::Object(int x, int y) : abx(x), aby(y) {
}

// 절대 좌표 기준 변경
void Object::setOffset(int x, int y) {
    abx = x;
    aby = y;
}

void Object::moveCursor(int x, int y) {
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD pos;
    pos.X = x * 2 + abx;
    pos.Y = y + aby;

    SetConsoleCursorPosition(hConsole, pos);
}

void Object::changeColor(int color) {
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void  Object::gotoxy(int x, int y) {
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
	SetConsoleCursorPosition(hConsole, pos);
}