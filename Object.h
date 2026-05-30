#pragma once
#include <Windows.h>

class Object {
protected:
    int abx;
    int aby;

public:
    Object(int x = 5, int y = 1);

    void setOffset(int x, int y);

    void moveCursor(int x, int y);

    static void gotoxy(int x, int y);
    static void changeColor(int color);
};