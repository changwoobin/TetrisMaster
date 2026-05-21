#include "GameState.h"

using namespace std;

// 초기화 시 절대 좌표도 Object로 전달
GameState::GameState(int level, int score, int lines, int abx, int aby)
	: Object(abx, aby), level(level), score(score), lines(lines), isTextPrinted(false) {
}

GameState::GameState() : GameState(0, 0, 0, 35, 7) {
}

int GameState::getScore() const { return score; }
int GameState::getLines() const { return lines; }
int GameState::getLevel() const { return level; }
void GameState::setScore(int score) { this->score = score; }
void GameState::setLines(int lines) { this->lines = lines; }
void GameState::setLevel(int level) { this->level = level; }

int GameState::show() {
	changeColor(GRAY);

	if (!isTextPrinted) {
		// 하드코딩된 좌표를 절대 좌표(abx, aby) 기준으로 오프셋 처리
		moveCursor(abx, aby);
		cout << "STAGE";

		moveCursor(abx, aby + 2);
		cout << "SCORE";

		moveCursor(abx, aby + 5);
		cout << "LINES";
		isTextPrinted = true;
	}

	moveCursor(abx + 6, aby);
	cout << level + 1;

	moveCursor(abx, aby + 3);
	cout << setw(10) << score;

	moveCursor(abx, aby + 6);
	cout << setw(10) << 20 - lines;

	return 0;
}