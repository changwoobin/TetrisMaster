#pragma once
#include "Object.h"
#include "Color.h"
#include <iostream>
using namespace std;

class Map : public Object {
private:
    int totalBlock[21][14];

public:
    Map(int abx = 5, int aby = 1);

    void draw(int level);
    void setMap(int row, int col, int data);

    int operator()(int x, int y);
};