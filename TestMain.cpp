#include "GameState.h"
#include "BlockMapManager.h"
#include "Block.h"
#include "TetrisGame.h"
#include <ctime>

int main(void) {
	srand((unsigned)time(NULL));

	TetrisGame game;
	game.setGame(1);
	game.play();

	return 0;
}