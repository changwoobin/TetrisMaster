#pragma once
#include <Windows.h>

class Object {
protected:
    // 절대 좌표 기준점
    int abx;
    int aby;

public:
    // 생성자에서 절대 좌표를 받을 수 있도록 기본값과 함께 추가
    Object(int x = 5, int y = 1);

    // 런타임에 절대 좌표 기준을 변경할 수 있는 함수
    void setOffset(int x, int y);

    void moveCursor(int x, int y);
    void changeColor(int color);
};