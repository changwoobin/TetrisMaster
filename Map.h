#pragma once
#include "Object.h"
#include "Color.h"
#include <iostream>
using namespace std;

class Map : public Object {
private:
    int totalBlock[21][14];

public:
    Map();

    // 원본 show_total_block()
    void draw(int level); // level에 따라 테두리 색을 바꾸어서 level을 인자로 받음
    void setMap(int row, int col, int data);
    
    int operator()(int x, int y);
};