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
	for (i = 1; 1; i++)
	{	
		if (_kbhit())
		{
			manager.changeColor(BLACK);
			gotoxy(77, 23);

			keytemp = _getche();
			if (keytemp == EXT_KEY)
			{
				keytemp = _getche();
				switch (keytemp)
				{
				case KEY_UP:		//회전하기
					
					if (manager.canRotate())
					{
						manager.currentBlockRotate();
					}
					break;
				case KEY_LEFT:		//왼쪽으로 이동
					if (manager.getCurBlockX() > 1)
					{
						manager.moveLeft();
					}
					break;
				case KEY_RIGHT:		//오른쪽으로 이동

					if (manager.getCurBlockX() < 13)
					{
						manager.moveRight();
					}
					break;
				case KEY_DOWN:		//아래로 이동
					is_gameover = manager.moveBlock();
					manager.showCurBlock();
					break;
				}
			}
			else
			{
				switch (keytemp)
				{
				case 32:
					while (is_gameover == 0)
					{
						is_gameover = manager.moveBlock();
					}
					manager.showCurBlock();
					break;

				case 'c': case 'C':
					manager.holdCurrentBlock();
					break;

				case 27:
				case 'p': case 'P':
					manager.changeColor(WHITE);
					gotoxy(10, 10); std::cout << " ┌───────────────────┐ ";
					gotoxy(10, 11); std::cout << " │     P A U S E     │ ";
					gotoxy(10, 12); std::cout << " │ [R]esume  [Q]uit  │ ";
					gotoxy(10, 13); std::cout << " └───────────────────┘ ";

					while (true) {
						if (_kbhit()) {
							char pauseKey = _getch();

							if (pauseKey == 'r' || pauseKey == 'R' || pauseKey == 27) {
								break;
							}
							else if (pauseKey == 'q' || pauseKey == 'Q') {
								return 0;
							}
						}
						Sleep(50);
					}

					manager.showMap();
					manager.showHoldBox();
					manager.showCurBlock();
					break;
				}
			}
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
		gotoxy(77, 23);
		Sleep(15);
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
