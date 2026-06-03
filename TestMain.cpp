#include "GameState.h"
#include "BlockMapManager.h"
#include "Block.h"
#include "TetrisGame.h"
#include "ConsoleRenderer.h"
#define _CRTDBG_MAP_ALLOC


int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int level = 0;

	TetrisGame game;
	
	while (1) {
		int chosen = runTetrisMaster(&level);

		if (chosen == 1) {
			showMultiInfo();
			if (game.multiplay() == 0) {
				continue;
			}
		}
		else if (chosen == 0) {
			showInfo();
			game.setGame(level-1);
			if (game.play() == 0) {
				continue;
			}
		}
		else if (chosen == -1) {
			break;
		}
	}
	return 0;
}