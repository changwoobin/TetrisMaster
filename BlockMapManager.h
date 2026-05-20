#pragma once
#include "Object.h"
#include "Block.h"
#include "Stage.h"
#include "Map.h"
#include "GameState.h"
#include <vector>
#include <iomanip>

class BlockMapManager : public Object
{
	GameState gameState;
	std::vector<Block> blocks;		// 과거에 진행된 블록, 현재 블록, 다음 블록 모두 저장
	Map map;
	int curBlock;					// 현재	진행 중인 블록의 인덱스
public:
	BlockMapManager();

	void showMap();

	int addBlock();			// blocks 벡터에 block 추가
	int startBlock();		// block 진행 시작

	GameState& getGameState();
	bool canPlace();		// 블록을 놓을 수 있으면 true 반환(strike_check)
	int checkFullLine();	
	int mergeBlock();		
	int moveBlock();
	void showCurBlock();
	void showNextBlock();
	void currentBlockRotate();
	int canRotate();
	int getCurBlockX() const;
	void moveRight();
	void moveLeft();
};

