#pragma once
#include "Object.h"

class GameState : public Object
{
	int score;
	int lines;
public:
	int show();
};

