#include "PlaySpace.h"


PlaySpace::PlaySpace()
{
	boardOutline.setFillColor(sf::Color::Black);
	boardOutline.setSize(sf::Vector2f(320, 640));
	boardOutline.setOutlineThickness(3);
	boardOutline.setPosition(BOARD_POS_X, BOARD_POS_Y);

	forceDown = false;
	floating = false;
	blockInPlay = true;
	lose = false;
	pausePlay = false;
	checkBreaks = true;
	moved = false;
	topFilled = false;
	addToScore = 0;
	nextBlock = rand() % TOTAL_BLOCK_TYPES;

	backgroundText.loadFromFile("background.png");
	background.setTexture(backgroundText);
	background.setPosition(0, 0);
	background.setScale(1.5, 1.5);
	background.setColor(sf::Color(255, 255, 255, 30));

	//clear board
	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			board[y][x] = 0;
			blockColors[y][x] = 0;
		}

	blockRotation = longBlock;
	int testBoard[BOARD_HEIGHT][BOARD_WIDTH] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 2, 2, 2, 0, 0, 0,
		0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 1
	};
	//for testing
	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			board[y][x] = testBoard[y][x];

			if (testBoard[y][x])
				blockColors[y][x] = 1;
		}
}

void PlaySpace::reset()
{
	for (int y = 0; y < BOARD_HEIGHT; ++y)
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			board[y][x] = 0;
			blockColors[y][x] = 0;
			blockInPlay = false;
		}

	generateNextBlock();

	forceDown = false;
	floating = false;
	lose = false;
	pausePlay = false;
	checkBreaks = true;
	moved = false;
	topFilled = false;
	addToScore = 0;
}

void PlaySpace::drawPlaySpace(sf::RenderWindow& window)
{
	window.draw(boardOutline);
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			sf::RectangleShape block;
			block.setSize(sf::Vector2f(32, 32));
			block.setPosition(32.f * x + BOARD_POS_X, 32.f * y + BOARD_POS_Y);
			block.setOutlineColor(sf::Color::White);
			block.setOutlineThickness(1);

			if (blockColors[y][x] == 0)
			{
				block.setOutlineColor(sf::Color(10, 10, 10, 150));
				block.setFillColor(sf::Color::Transparent);
				window.draw(block);
			}

			else if (blockColors[y][x] == 1)
			{
				if (speedLevel == 1)
					block.setFillColor(sf::Color::Red);

				else if (speedLevel == 2)
					block.setFillColor(sf::Color::Cyan);

				else if (speedLevel == 3)
					block.setFillColor(sf::Color::Green);

				else
					block.setFillColor(sf::Color::Magenta);

				window.draw(block);
			}

			else if (blockColors[y][x] == 2)
			{		
				if (speedLevel == 1)
					block.setFillColor(sf::Color::Green);

				else if (speedLevel == 2)
					block.setFillColor(sf::Color::Blue);

				else if (speedLevel == 3)
					block.setFillColor(sf::Color::Yellow);

				else
					block.setFillColor(sf::Color::Cyan);

				window.draw(block);
			}

			else if (blockColors[y][x] == 3)
			{
				if (speedLevel == 1)
					block.setFillColor(sf::Color::Blue);

				else if (speedLevel == 2)
					block.setFillColor(sf::Color::Red);

				else
					block.setFillColor(sf::Color::Green);

				window.draw(block);
			}

			else if (blockColors[y][x] == 4)
			{
				block.setFillColor(sf::Color::Magenta);
				window.draw(block);
			}

			else if (blockColors[y][x] == 5)
			{
				block.setFillColor(sf::Color::Cyan);
				window.draw(block);
			}

			else if (blockColors[y][x] == 6)
			{
				block.setFillColor(sf::Color::Yellow);
				window.draw(block);
			}

			else if (blockColors[y][x] == 7)
			{
				block.setFillColor(sf::Color::White);
				window.draw(block);
			}
		}
	}
}

void PlaySpace::moveBlock(int direction, Sound& sound)
{
	forceDown = false;
	bool collision = false;
	char buff[BOARD_HEIGHT][BOARD_WIDTH];
	char colorBuff[BOARD_HEIGHT][BOARD_WIDTH];
	switch (direction)
	{
		case right:
			moved = true;
			//copy to block buffer-------------------
			for (int j = 0; j < BOARD_HEIGHT; ++j)
				for (int i = 0; i < BOARD_WIDTH; ++i)
				{
					if (board[j][i] == 2)
					{
						buff[j][i] = 2;
						colorBuff[j][i] = blockColors[j][i];
					}

					else
					{
						buff[j][i] = 0;
						colorBuff[j][i] = 0;
					}
				}
			//----------------------------------------


			//test for collision-----------------------
			for (int j = 0; j < BOARD_HEIGHT; ++j)
				for (int i = 0; i < BOARD_WIDTH; ++i)
				{
					if (board[j][i] == 2 && board[j][i + 1] == 1)
						collision = true;
				}

			//check bounds
			for (int i = 0; i < BOARD_HEIGHT; ++i)
				if (board[i][BOARD_WIDTH - 1] == 2)
					collision = true;
			//-----------------------------------------

			if (collision == false)
			{
				sound.playSound(soundType::moveSound);
				//clear old block
				for (int j = 0; j < BOARD_HEIGHT; ++j)
					for (int i = 0; i < BOARD_WIDTH; ++i)
					{
						if (board[j][i] == 2)
						{
							board[j][i] = 0;
							blockColors[j][i] = 0;
						}
					}

				//copy saved block into shifted position
				for (int j = 0; j < BOARD_HEIGHT; ++j)
					for (int i = 0; i < BOARD_WIDTH; ++i)
					{
						if (buff[j][i] == 2)
						{
							board[j][i + 1] = buff[j][i];
							blockColors[j][i + 1] = colorBuff[j][i];
						}
					}
			}
			break; //end of right

		case down:
			forceDown = true;
			forceMoveDown(sound);
			break; //end of down

		case left:
			moved = true;
			//copy to block buffer-------------------
			buff[BOARD_HEIGHT][BOARD_WIDTH];
			for (int j = 0; j < BOARD_HEIGHT; ++j)
				for (int i = 0; i < BOARD_WIDTH; ++i)
				{
					if (board[j][i] == 2)
					{
						buff[j][i] = 2;
						colorBuff[j][i] = blockColors[j][i];
					}

					else
					{
						buff[j][i] = 0;
						colorBuff[j][i] = 0;
					}
				}
			//----------------------------------------


			//test for collision-----------------------
			for (int j = 0; j < BOARD_HEIGHT; ++j)
				for (int i = 0; i < BOARD_WIDTH; ++i)
				{
					if (board[j][i] == 2 && board[j][i - 1] == 1)
						collision = true;
				}

			//check bounds
			for (int i = 0; i < BOARD_HEIGHT; ++i)
				if (board[i][0] == 2)
					collision = true;
			//-----------------------------------------


			if (collision == false)
			{
				sound.playSound(soundType::moveSound);
				//clear old block
				for (int j = 0; j < BOARD_HEIGHT; ++j)
					for (int i = 0; i < BOARD_WIDTH; ++i)
					{
						if (board[j][i] == 2)
						{
							board[j][i] = 0;
							blockColors[j][i] = 0;
						}
					}

				//copy saved block into shifted position
				for (int j = 0; j < BOARD_HEIGHT; ++j)
					for (int i = 0; i < BOARD_WIDTH; ++i)
					{
						if (buff[j][i] == 2)
						{
							board[j][i - 1] = buff[j][i];
							blockColors[j][i - 1] = colorBuff[j][i];
						}
					}
			}
			break; //end of left

		case up:
			rotateBlock(sound);
			break;

		case skipDown:
			forceDown = true;
			while (blockInPlay == true && floating == false)
			{
				for (int j = BOARD_HEIGHT - 1; j > 0; --j)
				{
					for (int i = BOARD_WIDTH - 1; i >= 0; --i)
					{
						if (board[j - 1][i] == 2)
						{
							board[j][i] = board[j - 1][i];
							board[j - 1][i] = 0;

							blockColors[j][i] = blockColors[j - 1][i];
							blockColors[j - 1][i] = 0;
						}
					}
				}
				checkCollisions(sound);
			}
			break;
	}
}

void PlaySpace::shiftBlocksDown(int blockType, int starting, Sound& sound)
{
	updateTime = updateClock.getElapsedTime();

	//update blocks that need to be moved
	if (blockType == nonMoveableBlock)
	{
		for (int j = starting - 1; j > 1; --j)
		{
			for (int i = BOARD_WIDTH - 1; i >= 0; --i)
			{
				if (board[j][i] == 1)
				{
					//block
					board[j + 1][i] = board[j][i];
					board[j][i] = 0;

					//block color
					blockColors[j + 1][i] = blockColors[j][i];
					blockColors[j][i] = 0;
				}
			}
		}
	}

	else if (blockType == moveableBlock)
	{
		if (floating == false)
		{
			if (updateTime.asMilliseconds() > MOVE_BLOCK_SPEED / speedLevel)
			{
				for (int j = BOARD_HEIGHT - 1; j > 0; --j)
				{
					for (int i = BOARD_WIDTH - 1; i >= 0; --i)
					{
						if (board[j - 1][i] == 2)
						{
							if (board[j][i] == 1)
							{
								for (int y = 0; y < BOARD_HEIGHT; ++y)
								{
									for (int x = 0; x < BOARD_WIDTH; ++x)
									{
										if (board[y][x] == 2)
										{
											board[y][x] = 1;
											blockInPlay = false;
										}
									}
								}
								floating = false;
								sound.playSound(soundType::landSound);

							}
						}
					}
				}

				for (int j = BOARD_HEIGHT - 1; j > 0; --j)
				{
					for (int i = BOARD_WIDTH - 1; i >= 0; --i)
					{
						if (board[j - 1][i] == blockType)
						{
							board[j][i] = board[j - 1][i];
							board[j - 1][i] = 0;

							blockColors[j][i] = blockColors[j - 1][i];
							blockColors[j - 1][i] = 0;
						}
					}
					updateClock.restart();
				}
			}
		}
	}
	moved = false;
}

void PlaySpace::checkTotalBlockBreaks(Sound& sound)
{
	if (rowsToDestroy <= 0)
	{
		checkBreaks = true;
		rowsToDestroy = 0;
	}
	--rowsToDestroy;

	int rows = 0;

	if (checkBreaks)
	{
		bool row = true;
		for (int i = BOARD_HEIGHT - 1; i >= 0; --i)
		{
			row = true;
			for (int j = BOARD_WIDTH - 1; j >= 0; --j)
			{
				if (board[i][j] != 1)
				{
					row = false;
					break;
				}
			}

			if (row)
				++rows;
		}
	}

	//add breaks to score
	addToScore = 0;
	if (rows > 0)
	{
		rowsToDestroy = rows;
		addToScore = rows;

		if (rows == 4)
			sound.playSound(soundType::tetrisSound);

		else
			sound.playSound(soundType::breakSound);

		checkBreaks = false;
	}
}

void PlaySpace::checkForBreak(sf::RenderWindow& window, Sound& sound)
{
	int destroyedRows = 0;
	int starting = 0;

	for (int j = BOARD_HEIGHT - 1; j >= 0; --j)
	{
		bool filledRow = true;
		breakingBlocks = true;
		for (int i = BOARD_WIDTH - 1; i >= 0; --i)
		{
			if (board[j][i] != 1)
			{
				filledRow = false;
				continue;
			}
			starting = j;
		}

		if (filledRow)
		{
			sf::Clock clock;
			sf::Time time;
			bool clearing = true;

			//save colors to buffer
			char colorBuff[BOARD_HEIGHT][BOARD_WIDTH];
			for (int k = 0; k < BOARD_HEIGHT; ++k)
				for (int i = 0; i < BOARD_WIDTH; ++i)
					colorBuff[k][i] = blockColors[k][i];

			while (clearing)
			{
				time = clock.getElapsedTime();

				for (int j = BOARD_HEIGHT - 1; j >= 0; --j)
				{
					bool flash = true;
					for (int i = BOARD_WIDTH - 1; i >= 0; --i)
					{
						if (board[j][i] != 1)
							flash = false;
					}

					//change block color
					if (flash)
						for (int k = 0; k < BOARD_WIDTH; ++k)
						{
							if (time.asMilliseconds() % 2 == 0)
								blockColors[j][k] = 7;

							else
								blockColors[j][k] = 0;
						}

				}

				window.clear();
				window.draw(background);
				drawPlaySpace(window);
				window.display();

				//change all colors back
				for (int j = 0; j < BOARD_HEIGHT; ++j)
					for (int i = 0; i < BOARD_WIDTH; ++i)
						blockColors[j][i] = colorBuff[j][i];

				if (time.asMilliseconds() >= 100)
					clearing = false;
			}

			for (int k = 0; k < BOARD_WIDTH; ++k)
			{
				board[j][k] = 0;
				blockColors[j][k] = 0;
			}
	
			shiftBlocksDown(nonMoveableBlock, starting, sound);
		}
	}
}

void PlaySpace::checkCollisions(Sound& sound)
{
	bool atBottom = false;

	if (floating == false)
		floatClock.restart();

	//check for block collision
	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		//if a moving block is detected in last row, make all moving blocks '2' non moving '1' blocks
		if (board[BOARD_HEIGHT - 1][i] == 2)
		{
			floatTime = floatClock.getElapsedTime();
			floating = true;
			atBottom = true; //flag that block has reached the bottom of board

			if (floatTime.asMilliseconds() > (FLOAT_TIME_BASE - speedLevel * 15) || forceDown == true) //wait to place block till "floatTime" has timed out or a forceDown event has occured
			{
				forceDown = false;
				for (int y = 0; y < BOARD_HEIGHT; ++y)
				{
					for (int x = 0; x < BOARD_WIDTH; ++x)
					{
						if (board[y][x] == 2)
						{
							board[y][x] = 1;
							blockInPlay = false;
						}
					}
				}
				floating = false;
			}
			sound.playSound(soundType::landSound);
			break;
		}
	}

	//collision with another block
	for (int j = 0; j < BOARD_HEIGHT - 1; ++j)
	{
		for (int i = 0; i < BOARD_WIDTH; ++i)
		{
			if (board[j][i] == 2 && board[j + 1][i] == 1) //current moving block 1 unit above a placed block?
			{
				floatTime = floatClock.getElapsedTime();
				floating = true;

				if (floatTime.asMilliseconds() >(FLOAT_TIME_BASE - speedLevel * 15) || forceDown == true) //wait to place block till "floatTime" has timed out or a forceDown event has occured
				{
					forceDown = false;
					for (int y = 0; y < BOARD_HEIGHT; ++y)
					{
						for (int x = 0; x < BOARD_WIDTH; ++x)
						{
							if (board[y][x] == 2)
							{
								board[y][x] = 1;
								blockInPlay = false;
							}
						}
					}
					floating = false;
					sound.playSound(soundType::landSound);
					break;
				}
			}
			
			if (atBottom == false && (floating == true && floatTime.asMilliseconds() > 0 && board[j][i] == 2 && board[j + 1][i] != 1) && moved) //cancel float time, block has moved to open space
				floating = false;

			if (breakingBlocks == false)
			{
				if (atBottom == true) //if the block is touching the bottom of the board, the float time cannot be canceled
					floating = true;
			}
		}
	}
}

bool PlaySpace::almostFull(Music& music)
{
	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		if (board[5][i] == 1)
		{
			if (topFilled == false)
				music.stop();

			topFilled = true;
			return true;
		}
	}

	if (topFilled == true) //reset music back to slow
		music.stop();

	topFilled = false;
	return false;
}

int PlaySpace::getNextBlock()
{
	return nextBlock;
}

void PlaySpace::generateNextBlock()
{
	nextBlock = rand() % TOTAL_BLOCK_TYPES;
}

void PlaySpace::clearBlock()
{
	for (int i = 0; i < BOARD_HEIGHT; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			if (board[i][j] == 2)
			{
				board[i][j] = 0;
				blockColors[i][j] = 0;
			}
		}
	}
}

bool PlaySpace::rotateCollision(char block[5][5], int startX, int startY)
{
	for (int j = 0; j < 5; ++j)
	{
		for (int i = 0; i < 5; ++i)
		{
			if (board[startY + j][startX + i] == 1 && block[j][i] == 2)
				return true;
		}
	}
	
	//no collision found
	return false;
}

void PlaySpace::rotateBlock(Sound& sound)
{
	//save block type to lookup how that block should look when rotated in the lookup table
	//increment block rotation each rotation
	int currentRotation = blockRotation;
	++blockRotation;

	if (blockRotation > 3)
		blockRotation = 0;

	int posX = 0;
	int posY = 0;

	int startX = 0;
	int startY = 0;

	int color = 0;

	switch (currentBlockType)
	{
	case longBlock:
		//get first block position
		for (int i = 0; i < BOARD_HEIGHT; ++i)
		{
			for (int j = 0; j < BOARD_WIDTH; ++j)
			{
				if (board[i][j] == 2)
				{
					posX = j;
					posY = i;
					color = blockColors[i][j];
					break;
				}
			}
		}

		if (blockRotation == 0 || blockRotation == 2)
		{
			startX = posX - 1;
			startY = posY - 3;

			char block[5][5] =
			{
				0, 0, 2, 0, 0,
				0, 0, 2, 0, 0,
				0, 0, 2, 0, 0,
				0, 0, 2, 0, 0,
				0, 0, 0, 0, 0
			};

			if (rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int i = 0; i < 5; ++i)
					for (int j = 0; j < 5; ++j)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}

		else if (blockRotation == 1 || blockRotation == 3)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				2, 2, 2, 2, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}
		sound.playSound(rotateSound);
		break;

	case lBlock:
		//get first block position
		for (int i = 0; i < BOARD_HEIGHT; ++i)
		{
			for (int j = 0; j < BOARD_WIDTH; ++j)
			{
				if (board[i][j] == 2)
				{
					posX = j;
					posY = i;
					color = blockColors[i][j];
					break;
				}
			}
		}

		if (blockRotation == 0)
		{
			startX = posX - 1;
			startY = posY - 3;

			char block[5][5] =
			{
				0, 0, 2, 0, 0,
				0, 0, 2, 0, 0,
				0, 0, 2, 2, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int i = 0; i < 5; ++i)
					for (int j = 0; j < 5; ++j)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}
		
		else if (blockRotation == 1)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				0, 0, 0, 0, 0,
				0, 2, 2, 2, 0,
				0, 2, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}

		else if (blockRotation == 2)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				2, 2, 0, 0, 0,
				0, 2, 0, 0, 0,
				0, 2, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}

		else if (blockRotation == 3)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				0, 0, 2, 0, 0,
				2, 2, 2, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}
		sound.playSound(rotateSound);
		break;

		case bLBlock:
			//get first block position
			for (int i = 0; i < BOARD_HEIGHT; ++i)
			{
				for (int j = 0; j < BOARD_WIDTH; ++j)
				{
					if (board[i][j] == 2)
					{
						posX = j;
						posY = i;
						color = blockColors[i][j];
						break;
					}
				}
			}

			if (blockRotation == 0)
			{
				startX = posX - 1;
				startY = posY - 3;

				char block[5][5] =
				{
					0, 2, 0, 0, 0,
					0, 2, 0, 0, 0,
					2, 2, 0, 0, 0,
					0, 0, 0, 0, 0,
					0, 0, 0, 0, 0
				};

				if (rotateCollision(block, startX, startY) == false)
				{
					clearBlock();

					//copy new block
					for (int i = 0; i < 5; ++i)
						for (int j = 0; j < 5; ++j)
						{
							if (block[j][i] == 2)
							{
								board[startY + j][startX + i] = 2;
								blockColors[startY + j][startX + i] = color;
							}
						}
				}

				else
					blockRotation = currentRotation;
			}

			else if (blockRotation == 1)
			{
				startX = posX - 1;
				startY = posY - 1;

				char block[5][5] =
				{
					0, 0, 0, 0, 0,
					0, 2, 0, 0, 0,
					0, 2, 2, 2, 0,
					0, 0, 0, 0, 0,
					0, 0, 0, 0, 0
				};

				if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
				{
					clearBlock();

					//copy new block
					for (int j = 0; j < 5; ++j)
						for (int i = 0; i < 5; ++i)
						{
							if (block[j][i] == 2)
							{
								board[startY + j][startX + i] = 2;
								blockColors[startY + j][startX + i] = color;
							}
						}
				}

				else
					blockRotation = currentRotation;
			}

			else if (blockRotation == 2)
			{
				startX = posX - 1;
				startY = posY - 1;

				char block[5][5] =
				{
					0, 2, 2, 0, 0,
					0, 2, 0, 0, 0,
					0, 2, 0, 0, 0,
					0, 0, 0, 0, 0,
					0, 0, 0, 0, 0
				};

				if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
				{
					clearBlock();

					//copy new block
					for (int j = 0; j < 5; ++j)
						for (int i = 0; i < 5; ++i)
						{
							if (block[j][i] == 2)
							{
								board[startY + j][startX + i] = 2;
								blockColors[startY + j][startX + i] = color;
							}
						}
				}

				else
					blockRotation = currentRotation;
			}

			else if (blockRotation == 3)
			{
				startX = posX - 1;
				startY = posY - 1;

				char block[5][5] =
				{
					0, 0, 0, 0, 0,
					2, 2, 2, 0, 0,
					0, 0, 2, 0, 0,
					0, 0, 0, 0, 0,
					0, 0, 0, 0, 0
				};

				if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
				{
					clearBlock();

					//copy new block
					for (int j = 0; j < 5; ++j)
						for (int i = 0; i < 5; ++i)
						{
							if (block[j][i] == 2)
							{
								board[startY + j][startX + i] = 2;
								blockColors[startY + j][startX + i] = color;
							}
						}
				}

				else
					blockRotation = currentRotation;
			}
			sound.playSound(rotateSound);
			break;

	case tBlock:
		//get first block position
		for (int i = 0; i < BOARD_HEIGHT; ++i)
		{
			for (int j = 0; j < BOARD_WIDTH; ++j)
			{
				if (board[i][j] == 2)
				{
					posX = j;
					posY = i;
					color = blockColors[i][j];
					break;
				}
			}
		}

		if (blockRotation == 0)
		{
			startX = posX - 1;
			startY = posY - 3;

			char block[5][5] =
			{
				0, 0, 0, 0, 0,
				0, 2, 0, 0, 0,
				2, 2, 2, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int i = 0; i < 5; ++i)
					for (int j = 0; j < 5; ++j)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}

		else if (blockRotation == 1)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				0, 0, 2, 0, 0,
				0, 0, 2, 2, 0,
				0, 0, 2, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}

		else if (blockRotation == 2)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				0, 0, 0, 0, 0,
				2, 2, 2, 0, 0,
				0, 2, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}

		else if (blockRotation == 3)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				0, 2, 0, 0, 0,
				2, 2, 0, 0, 0,
				0, 2, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}
		sound.playSound(rotateSound);
		break;

		case angledBlock:
		//get first block position
		for (int i = 0; i < BOARD_HEIGHT; ++i)
		{
			for (int j = 0; j < BOARD_WIDTH; ++j)
			{
				if (board[i][j] == 2)
				{
					posX = j;
					posY = i;
					color = blockColors[i][j];
					break;
				}
			}
		}

		if (blockRotation == 0 || blockRotation == 2)
		{
			startX = posX - 1;
			startY = posY - 3;

			char block[5][5] =
			{
				0, 2, 0, 0, 0,
				0, 2, 2, 0, 0,
				0, 0, 2, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int i = 0; i < 5; ++i)
					for (int j = 0; j < 5; ++j)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}

		else if (blockRotation == 1 || blockRotation == 3)
		{
			startX = posX - 1;
			startY = posY - 1;

			char block[5][5] =
			{
				0, 0, 0, 0, 0,
				0, 2, 2, 0, 0,
				2, 2, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0
			};

			if (posX > 0 && posX < BOARD_WIDTH - 2 && rotateCollision(block, startX, startY) == false)
			{
				clearBlock();

				//copy new block
				for (int j = 0; j < 5; ++j)
					for (int i = 0; i < 5; ++i)
					{
						if (block[j][i] == 2)
						{
							board[startY + j][startX + i] = 2;
							blockColors[startY + j][startX + i] = color;
						}
					}
			}

			else
				blockRotation = currentRotation;
		}
		sound.playSound(rotateSound);
		break;

	case squareBlock:
		sound.playSound(rotateSound);
		break;
	}
}

void PlaySpace::genBlock()
{
	if (blockInPlay == false)
	{
		currentBlockType = nextBlock;
		blockRotation = 0;

		int color = nextBlock + 1;
		if (nextBlock == longType)
		{
			LongBlock* longBlock = new LongBlock;
			const char* newBlock = longBlock->getBlockArray();

			for (int i = 0; i < 25; ++i)
			{
				//if (board[0][i - (i / 5) * 5] == 1)
					//lose = true;

				if (newBlock[i] == 2) //only place new blocks, not empty spaces
				{
					if (board[i / 5 + longBlock->getStart()][3 + i - (i / 5) * 5] == 1) //block in the way of new block, lost
						lose = true;

					board[i / 5 + longBlock->getStart()][3 + i - (i / 5) * 5] = newBlock[i];
					blockColors[i / 5 + longBlock->getStart()][3 + i - (i / 5) * 5] = color;
				}
			}
			delete longBlock;
		}

		else if (nextBlock == squareType)
		{
			SquareBlock* squareBlock = new SquareBlock;
			const char* newBlock = squareBlock->getBlockArray();

			for (int i = 0; i < 25; ++i)
			{
				if (board[0][i - (i / 5) * 5] == 1)
					lose = true;

				if (newBlock[i] == 2) //only place new blocks, not empty spaces
				{
					if (board[i / 5 + squareBlock->getStart()][3 + i - (i / 5) * 5] == 1) //block in the way of new block, lost
						lose = true;

					board[i / 5 + squareBlock->getStart()][3 + i - (i / 5) * 5] = newBlock[i];
					blockColors[i / 5 + squareBlock->getStart()][3 + i - (i / 5) * 5] = color;
				}
			}
			delete squareBlock;
		}

		else if (nextBlock == lType)
		{
			LBlock* lBlock = new LBlock;
			const char* newBlock = lBlock->getBlockArray();

			for (int i = 0; i < 25; ++i)
			{
				if (board[0][i - (i / 5) * 5] == 1)
					lose = true;

				if (newBlock[i] == 2) //only place new blocks, not empty spaces
				{
					if (board[i / 5 + lBlock->getStart()][3 + i - (i / 5) * 5] == 1) //block in the way of new block, lost
						lose = true;

					board[i / 5 + lBlock->getStart()][3 + i - (i / 5) * 5] = newBlock[i];
					blockColors[i / 5 + lBlock->getStart()][3 + i - (i / 5) * 5] = color;
				}
			}
			delete lBlock;
		}

		else if (nextBlock == bLType)
		{
			BackwardsLBlock* bLBlock = new BackwardsLBlock;
			const char* newBlock = bLBlock->getBlockArray();

			for (int i = 0; i < 25; ++i)
			{
				if (board[0][i - (i / 5) * 5] == 1)
					lose = true;

				if (newBlock[i] == 2) //only place new blocks, not empty spaces
				{
					if (board[i / 5 + bLBlock->getStart()][3 + i - (i / 5) * 5] == 1) //block in the way of new block, lost
						lose = true;

					board[i / 5 + bLBlock->getStart()][3 + i - (i / 5) * 5] = newBlock[i];
					blockColors[i / 5 + bLBlock->getStart()][3 + i - (i / 5) * 5] = color;
				}
			}
			delete bLBlock;
		}

		else if (nextBlock == tType)
		{
			TBlock* tBlock = new TBlock;
			const char* newBlock = tBlock->getBlockArray();

			for (int i = 0; i < 25; ++i)
			{
				if (board[0][i - (i / 5) * 5] == 1)
					lose = true;

				if (newBlock[i] == 2) //only place new blocks, not empty spaces
				{
					if (board[i / 5 + tBlock->getStart()][3 + i - (i / 5) * 5] == 1) //block in the way of new block, lost
						lose = true;

					board[i / 5 + tBlock->getStart()][3 + i - (i / 5) * 5] = newBlock[i];
					blockColors[i / 5 + tBlock->getStart()][3 + i - (i / 5) * 5] = color;
				}
			}
			delete tBlock;
		}

		else if (nextBlock == angledType)
		{
			AngledBlock* angledBlock = new AngledBlock;
			const char* newBlock = angledBlock->getBlockArray();

			for (int i = 0; i < 25; ++i)
			{
				if (board[0][i - (i / 5) * 5] == 1)
					lose = true;

				if (newBlock[i] == 2) //only place new blocks, not empty spaces
				{
					if (board[i / 5 + angledBlock->getStart()][3 + i - (i / 5) * 5] == 1) //block in the way of new block, lost
						lose = true;

					board[i / 5 + angledBlock->getStart()][3 + i - (i / 5) * 5] = newBlock[i];
					blockColors[i / 5 + angledBlock->getStart()][3 + i - (i / 5) * 5] = color;
				}
			}
			delete angledBlock;
		}

		blockInPlay = true;	
		generateNextBlock();
	}
}

int PlaySpace::checkLost(sf::RenderWindow& window, Sound& sound)
{
	if (lose)
	{
		sound.playSound(soundType::loseSound);

		for (int j = BOARD_HEIGHT - 1; j >= 0; --j)
		{
			for (int i = BOARD_WIDTH - 1; i >= 0; --i)
			{
				board[j][i] = 1;
				blockColors[j][i] = 3;
			}

			window.clear();
			drawPlaySpace(window);
			window.display();
		}

		lose = false;
		blockInPlay = false;
		return 1;
	}
	return 0;
}

void PlaySpace::forceMoveDown(Sound& sound)
{
	checkCollisions(sound); //check for block collsion before moving down, otherwise will go through already placed blocks
	for (int j = BOARD_HEIGHT - 1; j > 0; --j)
	{
		for (int i = BOARD_WIDTH - 1; i >= 0; --i)
		{
			if (board[j - 1][i] == 2)
			{
				board[j][i] = board[j - 1][i];
				board[j - 1][i] = 0;

				blockColors[j][i] = blockColors[j - 1][i];
				blockColors[j - 1][i] = 0;
			}
		}
	}
}

void PlaySpace::setLevel(int currentLevel)
{
	speedLevel = currentLevel;
}

int PlaySpace::update(sf::RenderWindow& window, int direction, Sound& sound, int currentLevel)
{
	addToScore = 0;

	//speedLevel = currentLevel;
	moveBlock(direction, sound);
	genBlock();
	
	checkCollisions(sound);
	checkTotalBlockBreaks(sound);
	checkForBreak(window, sound);

	//blocks won't be shifted down or placed till timed out (or cancelled)
	shiftBlocksDown(moveableBlock, 0, sound);

	if (checkLost(window, sound))
		return -1; //sends lost signal to main game

	return addToScore;
}