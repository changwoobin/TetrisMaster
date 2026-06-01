#include "GameState.h"
#include "BlockMapManager.h"
#include "Block.h"
#include "TetrisGame.h"
#include "start.h"

int main(void) {
	TetrisGame game;
	game.setGame(0);

	while (1) {
		int chosen = runTetrisMaster();

		if (chosen == 0) {
			if (game.play() == 0) {
				continue;
			}
		}
		else if (chosen == 1) {
			if (game.multiplay() == 0) {
				continue;
			}
		}
		else if (chosen == -1) {
			break;
		}
	}
	return 0;
}