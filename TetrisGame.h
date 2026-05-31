#pragma once
#include "GameState.h"
#include "BlockMapManager.h"
#include "Stage.h"

#define EXT_KEY			0xffffffe0	//확장키 인식값 
#define KEY_LEFT		0x4b
#define KEY_RIGHT		0x4d
#define KEY_UP			0x48
#define KEY_DOWN		0x50
#define KEY_SPACE		32

class TetrisGame
{
	GameState gameState;
	BlockMapManager manager;
	int level;
	int nextFlipTick;
	int nextRandomTick;
public:
 	TetrisGame();
	void setGame(int level);
	int play();
	int multiplay();
	void scheduleNextFlip(int cur);
	void scheduleNextRandom(int cur);
	void addLineAttack_to_Opponent(BlockMapManager& m, int lines);
	void printAttackCounter(int attackCounter, int x, int y);

	int handlePauseMenu(BlockMapManager& p1Manager, BlockMapManager* p2Manager = nullptr);
	void handleStageClear();
};

