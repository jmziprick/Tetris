#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "LongBlock.h"
#include "SquareBlock.h"
#include "LBlock.h"
#include "BackwardsLBlock.h"
#include "TBlock.h"
#include "AngledBlock.h"
#include "Sound.h"
#include "Music.h"

class PlaySpace
{
public:
	PlaySpace();
	void drawPlaySpace(sf::RenderWindow& window);
	int update(sf::RenderWindow& window, int direction, Sound& sound, int currentLevel);
	void forceMoveDown(Sound& sound);
	void reset();
	bool almostFull(Music& music);
	void setLevel(int currentLevel);

	//getters
	int getNextBlock();

private:
	void generateNextBlock();
	void moveBlock(int direction, Sound& sound);
	void shiftBlocksDown(int blockType, int starting, Sound& sound);
	void dropBlocksDown(int toPos);
	void checkForBreak(sf::RenderWindow& window, Sound& sound);
	void checkTotalBlockBreaks(Sound& sound);
	void checkCollisions(Sound& sound);
	void genBlock();
	int checkLost(sf::RenderWindow& window, Sound& sound);
	void rotateBlock(Sound& sound);
	void clearBlock();
	bool rotateCollision(char block[5][5], int startX, int startY);

	int blockRotation;
	int currentBlockType;

	bool blockInPlay;
	bool lose;
	bool pausePlay;
	bool checkBreaks;
	int addToScore;
	int nextBlock;

	int rowsToDestroy;

	int board[BOARD_HEIGHT][BOARD_WIDTH];
	char blockColors[BOARD_HEIGHT][BOARD_WIDTH];

	sf::RectangleShape boardOutline;

	sf::Texture backgroundText;
	sf::Sprite background;

	int speedLevel;

	sf::Clock updateClock;
	sf::Time updateTime;

	sf::Clock floatClock;
	sf::Time floatTime;
	bool floating;
	bool forceDown;
	bool breakingBlocks;
	bool moved; //flag for left and right movement, used for block to block collision
	bool topFilled; //flag for upper rows being filled, to speed music up

	enum blockTypes{ empty, nonMoveableBlock, moveableBlock };
	enum currentBlockTypes{longBlock, squareBlock, lBlock, bLBlock, tBlock, angledBlock};
};