#pragma once
#include "Object.h"
#include "Block.h"
#include "GameState.h"
#include "Blocks.h"
#include "Stage.h"
#include "Map.h"
#include <vector>
#include <iomanip>

class BlockMapManager
{
	Blocks& blocksStorage;		// 과거에 진행된 블록, 현재 블록, 다음 블록 모두 저장
	std::vector<Block>& blocks;

	Map map;

	int level;
	int curBlock;					// 현재	진행 중인 블록의 인덱스
	int holdShape;					// hold 블록 모양
	int remainHolds;				// hold 잔여 횟수
	int comboCount;					// 콤보 횟수
	int abx, aby;					// 절대좌표 오프셋

	int lastComboX;
	int lastComboY;
	int trashAnimTick;				//애니메이션 용 변수

public:
	BlockMapManager(int abx = 5, int aby = 1);

	void showMap();
	int addBlock();			// blocks 벡터에 block 추가
	int startBlock();		// block 진행 시작

	void holdCurrentBlock();
	void showHoldBox();
	void addHolds(int count);

	bool canPlace();		// 블록을 놓을 수 있으면 true 반환(strike_check)
	bool canPlace(int blockIndex);
	int checkFullLine(GameState& gameState);
	int mergeBlock(GameState& gameState);
	int mergeBlock(GameState& gameState, int blockIndex);
	int moveBlock(GameState& gameState);
	int moveBlock(GameState& gameState, int blockIndex);

	void showCurBlock();
	void showNextBlock();

	void currentBlockRotate();
	bool canRotate();

	int getCurBlockX() const;
	int getCurBlockY() const;
	int getCurBlockIndex() const;


	void moveRight();
	void moveLeft();
	int moveDown(GameState& gameState);
	int hardDrop(GameState& gameState);
	int hardDrop(GameState& gameState, int blockIndex, int x, int y);

	void flipMap();
	void addRandomLine();

	void erasePopUp();
	void drawPauseMenu();
	void drawStageClear();
	void drawCombo(int count);
	void drawTrashBag();

};

