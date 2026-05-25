#include "TetrisGame.h"
#include <conio.h>


void TetrisGame::setGame(int level)
{
	manager.getGameState().setLevel(level);
	this->level = level;
}

int TetrisGame::play()
{
	int i, is_gameover = 0;
	char keytemp;
	manager.showMap();
	manager.showNextBlock();
	manager.showHoldBox();
	manager.startBlock();
	manager.getGameState().show(); 
	bool prev_UP = false;
	bool prev_C = false;
	for (i = 1; 1; i++)
	{	
		manager.changeColor(BLACK);
		gotoxy(77, 23);

		

		/*keytemp = _getche();*/
		/*keytemp = _getche();*/

		if ((GetAsyncKeyState(VK_UP) & 0x8000) && !prev_UP) // 회전하기
		{
			if (manager.canRotate())
			{
				manager.currentBlockRotate();
			}
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) // 왼쪽으로 이동
		{
			if (manager.getCurBlockX() > 1)
			{
				manager.moveLeft();
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) // 오른쪽으로 이동
		{
			if (manager.getCurBlockX() < 13)
			{
				manager.moveRight();
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) // 아래로 이동
		{
			is_gameover = manager.moveBlock();
			manager.showCurBlock();
		}
		if (GetAsyncKeyState(KEY_SPACE) & 0x8000) // 스페이스바 (Hard Drop)
		{
			while (is_gameover == 0)
			{
				is_gameover = manager.moveBlock();
			}
			manager.showCurBlock();
		}
		else if ((GetAsyncKeyState('C') & 0x8000) && !prev_C) // 블록 홀드
		{
			manager.holdCurrentBlock();
		}
		if ((GetAsyncKeyState(27) & 0x8000) || (GetAsyncKeyState('P') & 0x8000)) // ESC 또는 P (일시정지)
		{
			manager.changeColor(WHITE);
			gotoxy(10, 10); std::cout << " ┌───────────────────┐ ";
			gotoxy(10, 11); std::cout << " │      P A U S E     │ ";
			gotoxy(10, 12); std::cout << " │ [R]esume  [Q]uit  │ ";
			gotoxy(10, 13); std::cout << " └───────────────────┘ ";
			
			Sleep(200);

			while (true) {
				bool is_R_Pressed = (GetAsyncKeyState('R') & 0x8000) != 0;
				bool is_Q_Pressed = (GetAsyncKeyState('Q') & 0x8000) != 0;
				bool is_ESC_Pressed = (GetAsyncKeyState(27) & 0x8000) != 0;

				if (is_R_Pressed || is_ESC_Pressed) {
					break;
				}
				else if (is_Q_Pressed) {
					return 0; 
				}
				
				Sleep(30);
			}

			Sleep(200);

			manager.showMap();
			manager.showHoldBox();
			manager.showCurBlock();
		}
		if (i % Stage::data[level].speed == 0)
		{
			is_gameover = manager.moveBlock();
			manager.showCurBlock();
		}

		if (Stage::data[level].clearLine <= manager.getGameState().getLines())	//클리어 스테이지
		{
			level++;
			manager.getGameState().setLevel(level);
			manager.getGameState().setLines(0);
			manager.addHolds(1);
			manager.showMap();
			manager.getGameState().show();
			manager.showNextBlock();
			manager.showHoldBox();
		}
		if (is_gameover == 1)
		{
			return 1;
		}

		manager.changeColor(BLACK);
		prev_UP = (GetAsyncKeyState(VK_UP) & 0x8000);
		prev_C = (GetAsyncKeyState('C') & 0x8000);
		gotoxy(77, 23);
		Sleep(30);
		gotoxy(77, 23);
	}
	return 0;
}

void TetrisGame::gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(hConsole, pos);
}
