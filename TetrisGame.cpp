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
	manager.startBlock();
	manager.getGameState().show(); 
	for (i = 1; 1; i++)
	{	
		if (_kbhit())
		{
			keytemp = _getche();
			if (keytemp == EXT_KEY)
			{
				keytemp = _getche();
				switch (keytemp)
				{
				case KEY_UP:		//회전하기
					
					if (manager.canRotate() == 1)
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
					manager.showMap();
					break;
				}
			}
			if (keytemp == 32)	//스페이스바를 눌렀을때
			{
				while (is_gameover == 0)
				{
					is_gameover = manager.canPlace();
				}
				manager.showCurBlock();
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
			manager.showMap();
			manager.getGameState().show();
			manager.showNextBlock();
		}
		if (is_gameover == 1)
		{
			return 1;
		}
		gotoxy(77, 23);
		Sleep(15);
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
