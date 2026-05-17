#include "GameState.h"
#include "BlockMapManager.h"
#include "Block.h"
#include <ctime>

int main(void) {
	srand((unsigned)time(NULL));

	BlockMapManager manager;
	manager.showMap();
	manager.startBlock();
	manager.showNextBlock();

	return 0;
}