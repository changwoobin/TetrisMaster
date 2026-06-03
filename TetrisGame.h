#pragma once
#include "GameState.h"
#include "GameManager.h"
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
	GameManager manager;
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
	void addLineAttack_to_Opponent(GameManager& m, int lines);
	void printAttackCounter(int attackCounter, int x, int y);

	int handlePauseMenu(GameManager& p1Manager, GameManager* p2Manager = nullptr);
	void handleStageClear();
	void handleGameOver();
	void drawStageClear();
	void drawPauseMenu();
	void erasePopUp();
	void showWinner(int winner);
	void drawResultPopup(int abx, bool isWin);
};

