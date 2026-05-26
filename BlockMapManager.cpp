#include "BlockMapManager.h"


BlockMapManager::BlockMapManager(int abx, int aby)
	: Object(abx, aby), curBlock(-1), map(abx, aby), gameState(0, 0, 0, abx + 30, aby + 8),
	holdShape(-1), remainHolds(3) {
}

void BlockMapManager::showMap() {
	map.draw(gameState.getLevel());
	gameState.show();
}

int BlockMapManager::addBlock() {
	int shape;
	int stickProb = rand() % 100;

	if (stickProb <= Stage::data[gameState.getLevel()].stickRate) {
		shape = 0;
	}
	else {
		shape = (rand() % 6) + 1;
	}

	Block newBlock(shape, abx, aby);
	blocks.push_back(newBlock);

	return 0;
}

int BlockMapManager::startBlock() {
	if (blocks.size() == 0 || curBlock + 1 == blocks.size()) { 
		addBlock();
	}

	curBlock++;

	blocks[curBlock].setOffset(abx, aby);
	blocks[curBlock].setCord(5, -4);
	blocks[curBlock].show();

	return 0;
}

GameState& BlockMapManager::getGameState()
{
	return gameState;
}

bool BlockMapManager::canPlace() {
	int curBlockX = blocks[curBlock].getX();
	int curBlockY = blocks[curBlock].getY();
	int curBlockAngle = blocks[curBlock].getAngle();
	int curBlockShape = blocks[curBlock].getShape();
	bool isBlocked = false;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (curBlockX + j == 0 || curBlockX + j == 13) {
				isBlocked = true;
			}
			else if (curBlockY + i >= 0 && curBlockX + j >= 0) {
				isBlocked = (map(curBlockX+j, curBlockY+i) == 1) ? true : false;
			}

			if (isBlocked && Block::blocks[curBlockShape][curBlockAngle][i][j] == 1) {
				return false;
			}
		}
	}

	return true;
}

int BlockMapManager::checkFullLine() {

	int clearedLinesThisTurn = 0; // 이번 턴에 지운 줄 수

	for (int i = 0; i < 20; i++) {
		int j;
		for (j = 0; j < 13; j++) {
			if (map(j, i) == 0) {
				break;
			}
		}

		if (j == 13) {
			clearedLinesThisTurn++;
			
			map.draw(gameState.getLevel());
			changeColor(BLUE);
			moveCursor(1 * 2 + abx, i + aby);
			for (int j = 1; j < 13; j++) {
				cout << "□ ";
				Sleep(10);
			}

			moveCursor(1 * 2 + abx, i + aby);
			for (int j = 1; j < 13; j++) {
				cout << "  ";
				Sleep(10);
			}

			for (int k = i; k > 0; k--) {
				for (int j = 1; j < 13; j++) {
					map.setMap(k, j, map(j, k - 1));
				}
			}

			for (int j = 1; j < 13; j++) {
				map.setMap(0, j, 0);
			}
		}
	}
	if (clearedLinesThisTurn > 0) {
		comboCount++; // 콤보 증가

		int baseScore = 100 + (gameState.getLevel() * 10) + (rand() % 10);
		int finalScore = 0;

		// 1. 다중 클리어 보상 (몇 줄 지웠는지에 따라 배수 적용)
		switch (clearedLinesThisTurn) {
		case 1: finalScore = baseScore; break;
		case 2: finalScore = baseScore * 3; break;
		case 3: finalScore = baseScore * 5; break;
		case 4: finalScore = baseScore * 10; break;
		}

		// 2. 연속 콤보 보상 (2번 연속부터 추가 점수)
		if (comboCount > 1) {
			finalScore += (comboCount * 50);
		}

		int curLines = gameState.getLines();
		int curScore = gameState.getScore();
		gameState.setLines(curLines + clearedLinesThisTurn);
		gameState.setScore(curScore + finalScore);
		gameState.show();
	}
	else {
		comboCount = 0;
	}
	return 0;
}


int BlockMapManager::mergeBlock() {
	int curBlockX = blocks[curBlock].getX();
	int curBlockY = blocks[curBlock].getY();
	int curBlockAngle = blocks[curBlock].getAngle();
	int curBlockShape = blocks[curBlock].getShape();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			map.setMap(curBlockY + i, curBlockX + j, Block::blocks[curBlockShape][curBlockAngle][i][j] | map(curBlockX + j, curBlockY + i));
		}
	}

	checkFullLine();
	showMap();
	return 0;
}

int BlockMapManager::moveBlock() {
	blocks[curBlock].erase();

	blocks[curBlock].moveDown();
	if (!canPlace()) {
		if (blocks[curBlock].getY() <= 0) {
			blocks[curBlock].moveUp();
			return 1;
		}

		blocks[curBlock].moveUp();
		mergeBlock();

		addBlock();
		startBlock();
		showNextBlock();
		return 2;
	}

	return 0;
}

void BlockMapManager::showCurBlock()
{
	blocks[curBlock].show();
}

void BlockMapManager::showNextBlock() {
	int nextBlock = curBlock + 1;

	if (blocks.size() == nextBlock) {
		addBlock();
	}

	int nextBoxX = abx + 30;
	int nextBoxY = aby;

	changeColor((gameState.getLevel() + 1) % 6 + 1);
	for (int i = 1; i < 7; i++) {
		moveCursor(nextBoxX, nextBoxY + i - 1);
		for (int j = 0; j < 6; j++) {
			if (i == 1 || i == 6 || j == 0 || j == 5) {
				cout << "■ ";
			}
			else {
				cout << "  ";
			}
		}
	}

	blocks[nextBlock].setOffset(nextBoxX + 2, nextBoxY + 1);
	blocks[nextBlock].setCord(0, 0);
	blocks[nextBlock].show();
}

void BlockMapManager::currentBlockRotate()
{
	blocks[curBlock].erase();
	blocks[curBlock].rotate();
	blocks[curBlock].show();

}

bool BlockMapManager::canRotate()
{
	bool rv;
	blocks[curBlock].rotate();
	rv = canPlace();
	blocks[curBlock].rotate();
	blocks[curBlock].rotate();
	blocks[curBlock].rotate();
	return rv;
}

int BlockMapManager::getCurBlockX() const
{
	return blocks[curBlock].getX();
}

void BlockMapManager::moveRight()
{
	blocks[curBlock].erase();
	blocks[curBlock].moveRight();
	if (!canPlace()) {
		blocks[curBlock].moveLeft();
	}
	blocks[curBlock].show();
}

void BlockMapManager::moveLeft()
{
	blocks[curBlock].erase();
	blocks[curBlock].moveLeft();
	if (!canPlace()) {
		blocks[curBlock].moveRight();
	}
	blocks[curBlock].show();
}

void BlockMapManager::addHolds(int count) {
	remainHolds += count;
	showHoldBox(); // 횟수가 올랐으니 화면 즉시 갱신
}

void BlockMapManager::showHoldBox() {
	int holdBoxX = abx + 43; // hold 박스 위치 조정
	int holdBoxY = aby;

	changeColor(GRAY);
	moveCursor(holdBoxX, holdBoxY - 1);
	std::cout << "HOLD (" << remainHolds << ")  ";

	for (int i = 1; i < 7; i++) {
		moveCursor(holdBoxX, holdBoxY + i - 1);
		for (int j = 0; j < 6; j++) {
			if (i == 1 || i == 6 || j == 0 || j == 5) {
				std::cout << "■ ";
			}
			else {
				std::cout << "  ";
			}
		}
	}

	if (holdShape != -1) {
		Block hBlock(holdShape, holdBoxX + 2, holdBoxY + 1);
		hBlock.setCord(0, 0);
		hBlock.show();
	}
}

void BlockMapManager::holdCurrentBlock() {
	if (remainHolds <= 0) return;

	blocks[curBlock].erase();

	if (holdShape == -1) {
		holdShape = blocks[curBlock].getShape();
		startBlock();
		showNextBlock();
	}
	else {
		int tempShape = blocks[curBlock].getShape();
		blocks[curBlock] = Block(holdShape, abx, aby);
		blocks[curBlock].setCord(5, -4);
		holdShape = tempShape;
	}

	remainHolds--;     
	showHoldBox();     
	blocks[curBlock].show();
}