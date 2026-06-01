#include "TetrisGame.h"
#include <conio.h>


TetrisGame::TetrisGame() : gameState(0, 0, 0, 5, 1), manager(5, 1)
{
	level = 0;
	scheduleNextFlip(0);
	scheduleNextRandom(0);
	srand((unsigned)time(NULL));
}

void TetrisGame::setGame(int level)
{
	gameState.setLevel(level);
	this->level = level;
}

int TetrisGame::play()
{
	int i, is_gameover = 0;
	char keytemp;
	bool prev_UP = false;
	bool prev_C = false;
	bool prev_Space = false;


	manager.showMap();
	manager.showNextBlock();
	manager.showHoldBox();
	manager.startBlock();
	gameState.show();
	
	for (i = 1; 1; i++)
	{
		if (i >= nextFlipTick) {
			manager.flipMap();
			scheduleNextFlip(i);
			continue;
		}

		if (i >= nextRandomTick) {
			manager.addRandomLine();
			scheduleNextRandom(i);
			manager.showMap();
			continue;
		}
		Object::changeColor(BLACK);
		Object::gotoxy(77, 23);



		/*keytemp = _getche();*/
		/*keytemp = _getche();*/

		if ((GetAsyncKeyState(VK_UP) & 0x8000) && !prev_UP) // 회전하기
		{
			manager.currentBlockRotate();
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) // 왼쪽으로 이동
		{
			manager.moveLeft();
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) // 오른쪽으로 이동
		{
			manager.moveRight();
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) // 아래로 이동
		{
			is_gameover = manager.moveDown(gameState);
		}
		if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !prev_Space) // 스페이스바 (Hard Drop)
		{
			is_gameover = manager.hardDrop(gameState);
		}
		else if ((GetAsyncKeyState('C') & 0x8000) && !prev_C) // 블록 홀드
		{
			manager.holdCurrentBlock();
		}
		if ((GetAsyncKeyState(27) & 0x8000) || (GetAsyncKeyState('P') & 0x8000)) // ESC 또는 P (일시정지)
		{
			if (handlePauseMenu(manager) == 1) return 0; // 1(퇴근)이 반환되면 게임 종료
		}
		if (i % Stage::data[level].speed == 0)
		{
			is_gameover = manager.moveDown(gameState);
		}

		handleStageClear();

		if (is_gameover == 1)
		{
			Object::gotoxy(0, 29);
			return 1;
		}

		Object::changeColor(BLACK);
		prev_UP = (GetAsyncKeyState(VK_UP) & 0x8000);
		prev_C = (GetAsyncKeyState('C') & 0x8000);
		prev_Space = GetAsyncKeyState(VK_SPACE) & 0x8000;

		manager.drawTrashBag();

		Object::gotoxy(77, 23);
		Sleep(30);
		Object::gotoxy(77, 23);
	}
	return 0;
}

int TetrisGame::multiplay()
{
	int i, is_gameover = 0, is_guestGameover = 0;
	int attackCounter = 5, guestAttackCounter = 5;
	char keytemp;
	bool prev_UP = false;
	bool prev_C = false;
	bool prev_W = false;
	bool prev_M = false;
	bool prev_V = false;
	bool prev_Comma = false;
	bool prev_B = false;
	bool prev_Period = false;
	bool prev_N = false;
	bool prev_Slash = false;
	BlockMapManager guestManager(100, 1);
	GameState guestGameState(0, 0, 0, 100, 1);

	system("mode con: cols=200 lines=30");

	manager.showMap();
	manager.startBlock();
	manager.showNextBlock();
	manager.showHoldBox();
	gameState.show();

	guestManager.showMap();
	guestManager.startBlock();
	guestManager.showNextBlock();
	guestManager.showHoldBox();
	guestGameState.show();
	printAttackCounter(attackCounter, 5, 23);
	printAttackCounter(guestAttackCounter, 100, 23);
	for (i = 1; 1; i++)
	{
		/*if (i >= nextFlipTick) {
			manager.flipMap();
			scheduleNextFlip(i);
			continue;
		}

		if (i >= nextRandomTick) {
			manager.addRandomLine();
			scheduleNextRandom(i);
			manager.showMap();
			continue;
		}*/
		Object::changeColor(BLACK);
		Object::gotoxy(77, 23);



		/*keytemp = _getche();*/
		/*keytemp = _getche();*/

		// 플레이어 1 입력 처리
		if ((GetAsyncKeyState('W') & 0x8000) && !prev_W) // 회전하기
		{
			manager.currentBlockRotate();
		}
		if (GetAsyncKeyState('A') & 0x8000) // 왼쪽으로 이동
		{
			manager.moveLeft();
		}
		else if (GetAsyncKeyState('D') & 0x8000) // 오른쪽으로 이동
		{
			manager.moveRight();
		}
		if (GetAsyncKeyState('S') & 0x8000) // 아래로 이동
		{
			is_gameover = manager.moveDown(gameState);
			int clearedLines = gameState.getRecentClearedLines();
			if (clearedLines > 0) {
				addLineAttack_to_Opponent(guestManager, clearedLines);
			}
		}
		if ((GetAsyncKeyState('V') & 0x8000) && !prev_V) // 스페이스바 (Hard Drop)
		{
			is_gameover = manager.hardDrop(gameState);
			int clearedLines = gameState.getRecentClearedLines();
			if (clearedLines > 0) {
				addLineAttack_to_Opponent(guestManager, clearedLines);
			}
		}
		else if ((GetAsyncKeyState('C') & 0x8000) && !prev_C) // 블록 홀드
		{
			manager.holdCurrentBlock();
		}

		if ((GetAsyncKeyState('B') & 0x8000) && !prev_B)
		{
			if (attackCounter > 0) {
				attackCounter--;
				printAttackCounter(attackCounter, 5, 23);
				guestManager.flipMap();
			}
		}
		else if ((GetAsyncKeyState('N') & 0x8000) && !prev_N)
		{
			if (attackCounter > 0) {
				attackCounter--;
				printAttackCounter(attackCounter, 5, 23);
				guestManager.hardDrop(guestGameState, manager.getCurBlockIndex(), manager.getCurBlockX(), manager.getCurBlockY());
				manager.showMap();
				manager.showCurBlock();
			}
		}



		// 플레이어 2 입력 처리
		if ((GetAsyncKeyState(VK_UP) & 0x8000) && !prev_UP) // 회전하기
		{
			guestManager.currentBlockRotate();
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) // 왼쪽으로 이동
		{
			guestManager.moveLeft();
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) // 오른쪽으로 이동
		{
			guestManager.moveRight();
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) // 아래로 이동
		{
			is_guestGameover = guestManager.moveDown(guestGameState);
			int clearedLines = guestGameState.getRecentClearedLines();
			if (clearedLines > 0) {
				addLineAttack_to_Opponent(manager, clearedLines);
			}
		}
		if ((GetAsyncKeyState(VK_OEM_COMMA) & 0x8000) && !prev_Comma) // 스페이스바 (Hard Drop)
		{
			is_guestGameover = guestManager.hardDrop(guestGameState);
			int clearedLines = guestGameState.getRecentClearedLines();
			if (clearedLines > 0) {
				addLineAttack_to_Opponent(manager, clearedLines);
			}
		}
		else if ((GetAsyncKeyState('M') & 0x8000) && !prev_M) // 블록 홀드
		{
			guestManager.holdCurrentBlock();
		}
		if ((GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000) && !prev_Period)
		{
			if (guestAttackCounter > 0) {
				guestAttackCounter--;
				printAttackCounter(guestAttackCounter, 100, 23);
				manager.flipMap();
			}
		}
		else if ((GetAsyncKeyState(VK_OEM_2) & 0x8000) && !prev_Slash)
		{
			if (guestAttackCounter > 0) {
				guestAttackCounter--;
				printAttackCounter(guestAttackCounter, 100, 23);
				manager.hardDrop(gameState, guestManager.getCurBlockIndex(), guestManager.getCurBlockX(), guestManager.getCurBlockY());
				guestManager.showMap();
				guestManager.showCurBlock();
			}
		}





		if ((GetAsyncKeyState(27) & 0x8000) || (GetAsyncKeyState('P') & 0x8000)) // ESC 또는 P (일시정지)
		{
			if (handlePauseMenu(manager, &guestManager) == 1) return 0;
		}
		if (i % Stage::data[level].speed == 0)
		{
			is_gameover = manager.moveDown(gameState);
			is_guestGameover = guestManager.moveDown(guestGameState);
			int clearedLines = guestGameState.getRecentClearedLines();
			if (clearedLines > 0) {
				addLineAttack_to_Opponent(manager, clearedLines);
			}
			clearedLines = gameState.getRecentClearedLines();
			if (clearedLines > 0) {
				addLineAttack_to_Opponent(guestManager, clearedLines);
			}
		}

		if (is_gameover == 1)
		{
			Object::gotoxy(0, 29);
			return 1;
		}
		if (is_guestGameover == 1)
		{
			Object::gotoxy(0, 29);
			return 2;
		}

		Object::changeColor(BLACK);
		prev_UP = (GetAsyncKeyState(VK_UP) & 0x8000);
		prev_C = (GetAsyncKeyState('C') & 0x8000);
		prev_W = (GetAsyncKeyState('W') & 0x8000);
		prev_M = (GetAsyncKeyState('M') & 0x8000);
		prev_B = (GetAsyncKeyState('B') & 0x8000);
		prev_Period = (GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000);
		prev_Comma = (GetAsyncKeyState(VK_OEM_COMMA) & 0x8000);
		prev_V = (GetAsyncKeyState('V') & 0x8000);
		prev_N = (GetAsyncKeyState('N') & 0x8000);
		prev_Slash = (GetAsyncKeyState(VK_OEM_2) & 0x8000);
		manager.drawTrashBag();
		guestManager.drawTrashBag();
		Object::gotoxy(77, 23);
		Sleep(30);
		Object::gotoxy(77, 23);
	}
	return 0;
}

int TetrisGame::handlePauseMenu(BlockMapManager& p1Manager, BlockMapManager* p2Manager)
{
	p1Manager.drawPauseMenu(); // 팝업 띄우기
	Sleep(200);

	while (true) {
		bool is_R_Pressed = (GetAsyncKeyState('R') & 0x8000) != 0;
		bool is_Q_Pressed = (GetAsyncKeyState('Q') & 0x8000) != 0;
		bool is_ESC_Pressed = (GetAsyncKeyState(27) & 0x8000) != 0;

		if (is_R_Pressed || is_ESC_Pressed) {
			break; // 재개 (루프 탈출)
		}
		else if (is_Q_Pressed) {
			Object::gotoxy(0, 29);
			return 1; // 종료(퇴근) 신호 반환
		}
		Sleep(30);
	}

	Sleep(200);

	p1Manager.erasePopUp();

	// 팝업이 닫히면 다시 맵 복구
	p1Manager.showMap();
	p1Manager.showHoldBox();
	p1Manager.showCurBlock();

	if (p2Manager != nullptr) { // 멀티플레이일 경우 2P 화면도 복구
		p2Manager->showMap();
		p2Manager->showHoldBox();
		p2Manager->showCurBlock();
	}
	return 0; // 계속 진행 신호 반환
}

void TetrisGame::handleStageClear()
{
	if (Stage::data[level].clearLine <= gameState.getLines())
	{
		manager.drawStageClear();
		Sleep(1500);

		manager.erasePopUp();

		level++;
		gameState.setLevel(level);
		gameState.setLines(0);
		manager.addHolds(1);

		manager.showMap();
		gameState.show();
		manager.showNextBlock();
		manager.showHoldBox();
	}
}
void TetrisGame::scheduleNextFlip(int cur)
{
	int delay = 1333 + (rand() % 1334);
	nextFlipTick = cur + delay;
}

void TetrisGame::scheduleNextRandom(int cur)
{
	int delay = 1333 + (rand() % 1334);
	nextRandomTick = cur + delay;
}

void TetrisGame::addLineAttack_to_Opponent(BlockMapManager& m, int lines)
{
	for (int i = 0; i < lines; i++)
	{
		m.addRandomLine();
	}
	m.showMap();
	m.showCurBlock();
}

void TetrisGame::printAttackCounter(int attackCounter, int x, int y)
{
	Object::changeColor(WHITE);
	Object::gotoxy(x, y);
	std::cout << "Attack : " << attackCounter;
}
