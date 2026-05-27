#include "GameState.h"

using namespace std;

GameState::GameState(int level, int score, int lines) 
	: level(level), score(score), lines(lines), isTextPrinted(false) {

}

GameState::GameState() : GameState(0, 0, 0) {

}

int GameState::getScore() const {
	return score;
}

int GameState::getLines() const{
	return lines;
}

int GameState::getLevel() const
{
	return level;
}

void GameState::setScore(int score) {
	this->score = score;
}

void GameState::setLines(int lines) {
	this->lines = lines;
}

void GameState::setLevel(int level) {
	this->level = level;
}

int GameState::show() {
	changeColor(GRAY);

	if (!isTextPrinted) {
		moveCursor(35, 7);
		cout << "STAGE";

		moveCursor(35, 9);
		cout << "SCORE";

		moveCursor(35, 12);
		cout << "LINES";
	}

	moveCursor(41, 7);
	cout << level + 1;

	moveCursor(35, 10);
	cout << setw(10) << score;

	moveCursor(35, 13);
	cout << setw(10) << 20 - lines;

	return 0;
}