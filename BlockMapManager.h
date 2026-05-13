#pragma once
#include "Object.h"
#include "Block.h"
#include <vector>

class BlockMapManager
{
	std::vector<Block> blocks;
public:
	int startBlock();
	int strikeCheck();
	int mergeBlock();
	int moveBlock();
};

