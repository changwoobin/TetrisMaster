#pragma once
#include "Block.h"
#include <vector>
class Blocks
{
private:
	std::vector<std::vector<Block>*> blocks;
	static int blocksCount;
	Blocks() = default;
	Blocks(const Blocks&) = default;
	Blocks& operator=(const Blocks&) = default;
public:
	static Blocks& getInstance();

	void push_back(Block& block);
	std::vector<Block>& getBlocks();
	void drawBorder(int boxX, int boxY, int color);
	void drawBorder(int boxX, int boxY, int color, int remain);
};

