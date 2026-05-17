#pragma once
#include "Object.h"
#include "Color.h"
#include <iostream>
#include <iomanip>

class GameState : public Object
{
	int score;
	int lines;
	int level;
	bool isTextPrinted;
public:
	GameState();
	GameState(int level, int score, int lines);

	int getScore() const;
	int getLines() const;
	int getLevel() const;

	void setScore(int score);
	void setLines(int lines);
	void setLevel(int level);

	int show();
};

