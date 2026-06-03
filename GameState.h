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
	int recentClearedLines;
public:
	GameState();
	GameState(int level, int score, int lines, int abx = 5, int aby = 1);

	void reset();

	int getScore() const;
	int getLines() const;
	int getLevel() const;
	int getRecentClearedLines();

	void setScore(int score);
	void setLines(int lines);
	void setLevel(int level);
	void addLines(int lines);

	int show();
};