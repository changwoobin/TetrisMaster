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

    void reset();

    void draw(int level);
    void setMap(int row, int col, int data);

    void flip();
    bool stepGravity();
    void insertLine(int pos, int line[]);
    void clearLine(const int pos, const int level);

    int operator()(int x, int y);
};