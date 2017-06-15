#ifndef __tetris__Game__
#define __tetris__Game__

#include "Config.h"
#include "Block.h"
#include "Chunk.h"
#include "Field.h"

class Game
{
public:

	Field* field;
	Chunk* chunk;
	bool* deletableLines;

	Game();
	~Game();

	void makeChunk(int x, int y, int numbers[]);

	void moveLeft();
	void moveRight();
	void moveUnder();

	bool canMoveLeft();
	bool canMoveRight();
	bool canMoveUnder();

	bool canTurnRight();

	void copyBlocks();

	void turnLeft();
	void turnRight();

	bool isConflict();

	bool checkDeletableLines();
	void deleteDeletableLines();
};

#endif