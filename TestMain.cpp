#include "GameState.h"
#include "BlockMapManager.h"
#include "Block.h"
#include "TetrisGame.h"

int main(void) {
	TetrisGame game;
	game.setGame(0);
	game.play();

	return 0;
}