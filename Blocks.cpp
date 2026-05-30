#include "Blocks.h"
#include "TetrisGame.h"

int Blocks::blocksCount = 0;

Blocks& Blocks::getInstance()
{
	static Blocks instance;
	return instance;
}

void Blocks::push_back(Block& block)
{
    for (auto it = blocks.begin(); it != blocks.end(); it++) {
        (*it)->push_back(block);
    }	
}

std::vector<Block>& Blocks::getBlocks()
{
    blocks.push_back(new std::vector<Block>());
	return *blocks[blocksCount++];
}

void Blocks::drawBorder(int boxX, int boxY, int color)
{
    Object::changeColor(color);
    for (int i = 1; i < 7; i++) {
        Object::gotoxy(boxX, boxY + i - 1);
        for (int j = 0; j < 6; j++) {
            if (i == 1 || i == 6 || j == 0 || j == 5) {
                cout << "бс ";
            }
            else {
                cout << "  ";
            }
        }
    }
}

void Blocks::drawBorder(int boxX, int boxY, int color, int remain)
{
    Object::changeColor(color);
    Object::gotoxy(boxX, boxY - 1);
    std::cout << "HOLD (" << remain << ")  ";
    for (int i = 1; i < 7; i++) {
        Object::gotoxy(boxX, boxY + i - 1);
        for (int j = 0; j < 6; j++) {
            if (i == 1 || i == 6 || j == 0 || j == 5) {
                cout << "бс ";
            }
            else {
                cout << "  ";
            }
        }
    }
}