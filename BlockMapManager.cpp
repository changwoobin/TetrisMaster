#include "BlockMapManager.h"


BlockMapManager::BlockMapManager(int abx, int aby)
	: Object(abx, aby), curBlock(-1), map(abx, aby), gameState(0, 0, 0, 35, 7) {
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

	// 생성되는 블록에 현재 보드의 절대 좌표 주입
	Block newBlock(shape, abx, aby);
	blocks.push_back(newBlock);

	return 0;
}

int BlockMapManager::startBlock() {
	if (blocks.size() == 0) {
		addBlock();
	}

	curBlock++;

	// 본 게임 보드로 진입하므로, 절대 좌표 기준을 게임판(abx, aby)으로 리셋
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
	int l = 0;
	for (int i = 0; i < 20; i++) {
		int curLines = gameState.getLines();
		int curScore = gameState.getScore();

		int j;
		for (j = 0; j < 13; j++) {
			if (map(j, i) == 0) {
				break;
			}
		}

		if (j == 13) {
			curLines++;		
			
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

			curScore += 100 + (gameState.getLevel() * 10) + (rand() % 10);

			gameState.setScore(curScore);
			gameState.setLines(curLines);
			gameState.show();
		}
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

	// Next Box의 독립적인 절대 좌표
	int nextBoxX = 33;
	int nextBoxY = 1;

	changeColor((gameState.getLevel() + 1) % 6 + 1);
	for (int i = 1; i < 7; i++) {
		// 하드코딩 대신 설정한 좌표 기준으로 박스 출력
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

	// 넥스트 블록의 절대 좌표 기준점을 Next Box 내부로 일시 변경
	blocks[nextBlock].setOffset(nextBoxX + 2, nextBoxY + 1);
	// 변경된 기준점(Next Box 내부)에서의 상대 좌표 설정
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
