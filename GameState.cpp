#include "GameState.h"
#include "Stage.h"

using namespace std;

// 초기화 시 절대 좌표도 Object로 전달
GameState::GameState(int level, int score, int lines, int abx, int aby)
	: Object(abx, aby), level(level), score(score), lines(lines), isTextPrinted(false), recentClearedLines(0) {
}

void GameState::reset()
{
	score = 0;
	isTextPrinted = false;
	recentClearedLines = 0;
	lines = 0;
}

GameState::GameState() : GameState(0, 0, 0, 5, 1) {
}

int GameState::getScore() const { return score; }
int GameState::getLines() const { return lines; }
int GameState::getLevel() const { return level; }
int GameState::getRecentClearedLines() { 
	int temp = recentClearedLines;
	recentClearedLines = 0;
	return temp; 
}

void GameState::setScore(int score) { this->score = score; }
void GameState::setLines(int lines) { this->lines = lines; }
void GameState::addLines(int lines) { this->lines += lines; recentClearedLines = lines; }
void GameState::setLevel(int level) { this->level = level; }

int GameState::show() {
	changeColor(GRAY);

	if (!isTextPrinted) {
		moveCursor(15, 14);
		cout << "STAGE";

		moveCursor(15, 16);
		cout << "SCORE";

		moveCursor(15, 19);
		cout << "LINES";
		isTextPrinted = true;
	}

	moveCursor(36/2, 14);
	cout << level + 1;

	moveCursor(15, 17);
	cout << setw(10) << score;

	moveCursor(15, 20);
	cout << setw(10) << Stage::data[level].clearLine - lines;;

	return 0;
}