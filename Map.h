#pragma once
#include "Object.h"
#include "Color.h"
#include <iostream>
using namespace std;

class Map : public Object {
private:
    int totalBlock[21][14];

public:
    // 맵도 절대 좌표를 받을 수 있도록 수정
    Map(int abx = 5, int aby = 1);

    void draw(int level);
    void setMap(int row, int col, int data);

    int operator()(int x, int y);
};