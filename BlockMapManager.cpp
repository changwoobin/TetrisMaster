#include "BlockMapManager.h"

#include <conio.h>

BlockMapManager::BlockMapManager(int abx, int aby)
	: abx(abx), aby(aby), curBlock(-1), map(abx, aby), holdShape(-1), remainHolds(2), comboCount(0), level(0), blocksStorage(Blocks::getInstance()), blocks(Blocks::getInstance().getBlocks()){
}

void BlockMapManager::showMap() {
	map.draw(level);
}

int BlockMapManager::addBlock() {
	int shape;
	int stickProb = rand() % 100;

	if (stickProb <= Stage::data[level].stickRate) {
		shape = 0;
	}
	else {
		shape = (rand() % 6) + 1;
	}

	Block newBlock(shape, abx, aby);
	blocksStorage.push_back(newBlock);

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

bool BlockMapManager::canPlace(int blockIndex)
{
	int curBlockX = blocks[blockIndex].getX();
	int curBlockY = blocks[blockIndex].getY();
	int curBlockAngle = blocks[blockIndex].getAngle();
	int curBlockShape = blocks[blockIndex].getShape();
	bool isBlocked = false;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (curBlockX + j == 0 || curBlockX + j == 13) {
				isBlocked = true;
			}
			else if (curBlockY + i >= 0 && curBlockX + j >= 0) {
				isBlocked = (map(curBlockX + j, curBlockY + i) == 1) ? true : false;
			}

			if (isBlocked && Block::blocks[curBlockShape][curBlockAngle][i][j] == 1) {
				return false;
			}
		}
	}

	return true;
}

int BlockMapManager::checkFullLine(GameState& gameState) {

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
			
			map.clearLine(i, level);
		}
	}
	if (clearedLinesThisTurn > 0) {
		comboCount++; // 콤보 증가
		trashAnimTick = 30; 
		drawCombo(comboCount);

		int baseScore = 100 + (level * 10) + (rand() % 10);
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

		int curScore = gameState.getScore();
		gameState.addLines(clearedLinesThisTurn);
		gameState.setScore(curScore + finalScore);
		gameState.show();
	}
	else {
		comboCount = 0;
	}
	return 0;
}


int BlockMapManager::mergeBlock(GameState& gameState) {
	int curBlockX = blocks[curBlock].getX();
	int curBlockY = blocks[curBlock].getY();
	int curBlockAngle = blocks[curBlock].getAngle();
	int curBlockShape = blocks[curBlock].getShape();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			map.setMap(curBlockY + i, curBlockX + j, Block::blocks[curBlockShape][curBlockAngle][i][j] | map(curBlockX + j, curBlockY + i));
		}
	}

	checkFullLine(gameState);
	showMap();
	return 0;
}

int BlockMapManager::mergeBlock(GameState& gameState, int blockIndex)
{
	int curBlockX = blocks[blockIndex].getX();
	int curBlockY = blocks[blockIndex].getY();
	int curBlockAngle = blocks[blockIndex].getAngle();
	int curBlockShape = blocks[blockIndex].getShape();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			map.setMap(curBlockY + i, curBlockX + j, Block::blocks[curBlockShape][curBlockAngle][i][j] | map(curBlockX + j, curBlockY + i));
		}
	}

	checkFullLine(gameState);
	showMap();
	return 0;
}

int BlockMapManager::moveBlock(GameState& gameState) {
	blocks[curBlock].erase();

	blocks[curBlock].moveDown();
	if (!canPlace()) {
		if (blocks[curBlock].getY() <= 0) {
			blocks[curBlock].moveUp();
			return 1;
		}

		blocks[curBlock].moveUp();
		mergeBlock(gameState);

		addBlock();
		startBlock();
		showNextBlock();
		return 2;
	}

	return 0;
}

int BlockMapManager::moveBlock(GameState& gameState, int blockIndex)
{
	blocks[blockIndex].erase();

	blocks[blockIndex].moveDown();
	if (!canPlace(blockIndex)) {
		if (blocks[blockIndex].getY() <= 0) {
			blocks[blockIndex].moveUp();
			return 1;
		}

		blocks[blockIndex].moveUp();
		mergeBlock(gameState, blockIndex);


		return 2;
	}

	return 0;
}

void BlockMapManager::showCurBlock()
{
	blocks[curBlock].setOffset(abx, aby);
	blocks[curBlock].show();
}

void BlockMapManager::showNextBlock() {
	int nextBlock = curBlock + 1;

	if (blocks.size() == nextBlock) {
		addBlock();
	}

	int nextBoxX = 30;
	int nextBoxY = 0;

	blocksStorage.drawBorder(nextBoxX + abx, nextBoxY + aby, (level + 1) % 6 + 1);
	blocks[nextBlock].setOffset(abx, aby);
	blocks[nextBlock].setCord((nextBoxX + 2) / 2, nextBoxY + 1);
	blocks[nextBlock].show();
}

void BlockMapManager::currentBlockRotate()
{
	if (canRotate()) {
		blocks[curBlock].erase();
		blocks[curBlock].rotate();
		blocks[curBlock].show();
	}
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

int BlockMapManager::getCurBlockY() const
{
	return blocks[curBlock].getY();
}

int BlockMapManager::getCurBlockIndex() const
{
	return curBlock;
}

void BlockMapManager::moveRight()
{
	if (blocks[curBlock].getX() < 13)
	{
		blocks[curBlock].erase();
		blocks[curBlock].moveRight();
		if (!canPlace()) {
			blocks[curBlock].moveLeft();
		}
		blocks[curBlock].show();
	}
	
}

void BlockMapManager::moveLeft()
{
	if (blocks[curBlock].getX() > 1)
	{
		blocks[curBlock].erase();
		blocks[curBlock].moveLeft();
		if (!canPlace()) {
			blocks[curBlock].moveRight();
		}
		blocks[curBlock].show();
	}
}

int BlockMapManager::moveDown(GameState& gameState)
{
	int is_gameover = 0;
	is_gameover = moveBlock(gameState);
	showCurBlock();

	return is_gameover;
}

int BlockMapManager::hardDrop(GameState& gameState)
{
	int is_gameover = 0;
	while (is_gameover == 0)
	{
		is_gameover = moveBlock(gameState);
	}
	showCurBlock();

	return is_gameover;
}

int BlockMapManager::hardDrop(GameState& gameState, int blockIndex, int x, int y)
{

	int is_gameover = 0;
	int bX, bY;
	bX = blocks[blockIndex].getX();
	bY = blocks[blockIndex].getY();
	blocks[blockIndex].setCord(x, y);
	while (is_gameover == 0)
	{
		is_gameover = moveBlock(gameState, blockIndex);
	}

	blocks[blockIndex].setCord(bX, bY);
	showCurBlock();

	return is_gameover;
}

void BlockMapManager::addHolds(int count) {
	remainHolds += count;
	showHoldBox(); // 횟수가 올랐으니 화면 즉시 갱신
}

void BlockMapManager::showHoldBox() {
	int holdBoxX = 43; // hold 박스 위치 조정
	int holdBoxY = 0;

	blocksStorage.drawBorder(holdBoxX + abx, holdBoxY + aby, GRAY, remainHolds);

	if (holdShape != -1) {
		Block hBlock(holdShape, abx, aby);
		hBlock.setCord((holdBoxX + 2) / 2, holdBoxY + 1);
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

void BlockMapManager::flipMap()
{
	map.flip();
	showMap();
	Sleep(300);

	while (map.stepGravity()) {
		showMap();
		Sleep(45);
	}

	while (_kbhit()) _getch();
}

void BlockMapManager::addRandomLine()
{
	int line[12];

	for (int i = 0; i < 12; i++) {
		int randNum = rand() % 2;

		line[i] = randNum;
	}

	map.insertLine(19, line);
}

void BlockMapManager::drawPauseMenu() {
	int evX = (abx % 2 == 0) ? abx : abx + 1; 

	Object::changeColor(WHITE);
	Object::gotoxy(evX + 2, aby + 7); std::cout << "┌─────────────────────────────────┐";
	Object::gotoxy(evX + 2, aby + 8); std::cout << "│          휴  식  시  간         │";
	Object::gotoxy(evX + 2, aby + 9); std::cout << "│                                 │";
	Object::gotoxy(evX + 2, aby + 10);std::cout << "│  [R] 다시  일하러  가자!        │";
	Object::gotoxy(evX + 2, aby + 11);std::cout << "│                                 │";
	Object::gotoxy(evX + 2, aby + 12);std::cout << "│  [Q] 정말  퇴근하시겠습니까?    │";
	Object::gotoxy(evX + 2, aby + 13);std::cout << "└─────────────────────────────────┘";
}

void BlockMapManager::drawStageClear() {
	Object::changeColor(YELLOW);
	Object::gotoxy(abx + 2, aby + 7); std::cout << "┌────────────────┐";
	Object::gotoxy(abx + 2, aby + 8); std::cout << "│일일 할당량 달성│";
	Object::gotoxy(abx + 2, aby + 9); std::cout << "│                │";
	Object::gotoxy(abx + 2, aby + 10);std::cout << "│김씨! 다음 구역!│";
	Object::gotoxy(abx + 2, aby + 11);std::cout << "│(특근수당 HOLD) │";
	Object::gotoxy(abx + 2, aby + 12);std::cout << "└────────────────┘";
}

void BlockMapManager::drawCombo(int count) {
	if (lastComboX != 0 && lastComboY != 0) {
		Object::gotoxy(lastComboX, lastComboY);
		std::cout << "                ";
	}

	if (count > 1) {
		lastComboX = abx + 36 + (rand() % 5);
		lastComboY = aby + 7 + (rand() % 3);

		Object::gotoxy(lastComboX, lastComboY);

		Object::changeColor(YELLOW); std::cout << "★ ";
		Object::changeColor(RED);    std::cout << count << " 연속 처리 ";
		Object::changeColor(YELLOW); std::cout << "★";

	}
	else {
		lastComboX = 0;
		lastComboY = 0;
	}
}

void BlockMapManager::drawTrashBag() {
	int tx = abx + 43;
	int ty = aby + 14;

	Object::changeColor(DARK_GRAY);

	if (trashAnimTick <= 0) {
		Object::gotoxy(tx, ty);     std::cout << "   //\\\\   ";
		Object::gotoxy(tx, ty + 1); std::cout << "  /____\\  ";
		Object::gotoxy(tx, ty + 2); std::cout << " /      \\ ";
		Object::gotoxy(tx, ty + 3); std::cout << " \\______/ ";
		return;
	}

	trashAnimTick--;

	if (trashAnimTick % 10 < 5) {
		Object::gotoxy(tx, ty);     std::cout << "   //\\\\   ";
		Object::gotoxy(tx, ty + 1); std::cout << "  /____\\  ";
		Object::gotoxy(tx, ty + 2); std::cout << " /      \\ ";
		Object::gotoxy(tx, ty + 3); std::cout << " \\______/ ";
	}
	else {
		Object::gotoxy(tx, ty);     std::cout << "   //\\\\   ";
		Object::gotoxy(tx, ty + 1); std::cout << "  /~~~~\\  ";
		Object::gotoxy(tx, ty + 2); std::cout << " /      \\ ";
		Object::gotoxy(tx, ty + 3); std::cout << " \\______/ ";
	}
}

void BlockMapManager::erasePauseMenu() {
	int evX = (abx % 2 == 0) ? abx : abx + 1;

	for (int i = 0; i < 7; i++) {
		Object::gotoxy(evX + 2, aby + 7 + i);
		std::cout << "                                        "; 
	}
}
