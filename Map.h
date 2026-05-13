#pragma once
#include "Object.h"

class Map : public Object
{
	int totalBlock[21][14];
public:
	int draw();
};

