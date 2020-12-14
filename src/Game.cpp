#include "Game.h"


Game::Game()
{
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);
	score = 0;
	level = 1;
	blocksBroken = 0;
	remainingBlocks = BLOCKS_TILL_NEXT_LEVEL;
	gameState = 0;
}

Game::~Game()
{
}

void Game::init()
{
	srand((unsigned int)time(0));
	window.create(sf::VideoMode(1000, 800), "Tetris", sf::Style::Titlebar|sf::Style::Close);
	font.loadFromFile("Tetris.ttf");
	nintendoFont.loadFromFile("Nintendo.ttf");
	backgroundText.loadFromFile("background.png");
	background.setTexture(backgroundText);
	background.setPosition(0, 0);
	background.setScale(1.5, 1.5);
	background.setColor(sf::Color(255, 255, 255, 30));

	winText.loadFromFile("win.png");
	winSprite.setTexture(winText);

	playSpace.setLevel(1);
	win = false;
}

void Game::showLevel()
{
	std::string levelStr;
	std::stringstream ss;
	ss << level;
	ss >> levelStr;

	sf::Text text;
	text.setFont(font);
	text.setString(levelStr);
	text.setCharacterSize(60);
	text.setColor(sf::Color::Blue);
	text.setPosition(50, 120);
	window.draw(text);

	levelStr = "Level";
	text.setString(levelStr);
	text.setPosition(50, 70);
	window.draw(text);
}

void Game::showScore()
{
	std::string scoreStr;
	std::stringstream ss;
	ss << score;
	ss >> scoreStr;
	
	sf::Text text;
	text.setFont(font);
	text.setString(scoreStr);
	text.setCharacterSize(60);
	text.setColor(sf::Color::Blue);
	text.setPosition(50, 290);
	window.draw(text);

	scoreStr = "Score";
	text.setString(scoreStr);
	text.setPosition(50, 240);
	window.draw(text);
}

void Game::updateLevel()
{
	if (remainingBlocks <= 0)
	{
		++level;
		playSpace.setLevel(level);
		sound.playSound(soundType::levelUpSound);
		score += 1000;
		remainingBlocks = BLOCKS_TILL_NEXT_LEVEL;
	}
}

void Game::calculateScore()
{
	int tmpScore = playSpace.update(window, direction, sound, level);

	sf::Clock clock;
	sf::Time time;
	if (tmpScore == -1) //lost
	{
		music.stop();
		sound.playSound(soundType::gameOverSound);

		clock.restart();
		while (time.asSeconds() < 3)
			time = clock.getElapsedTime();

		if (score > WIN_SCORE)
			win = true;

		if (win)
		{
			gameState = 777;
			return;
		}

		gameState = 0;
		return;
	}

	else
	{
		blocksBroken += tmpScore;

		if (tmpScore > 0)
			remainingBlocks -= tmpScore;

		if (tmpScore == 4) //tetris, give bonus
			score += 100 * (level + 1);

		else
			score += tmpScore * 10 * (level + 1);
	}

	updateLevel();
}

void Game::showNextBlock()
{
	sf::RectangleShape box;
	box.setOutlineColor(sf::Color::Blue);
	box.setFillColor(sf::Color::Transparent);
	box.setSize(sf::Vector2f(161, 161));
	box.setOutlineThickness(2);
	box.setPosition(720, 150);

	int nextBlock = playSpace.getNextBlock();
	int nextBlockSpace[5][5];

	if (nextBlock == longType)
	{
		int block[5][5] =
		{
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0
		};

		//copy block type
		for (int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; ++j)
				nextBlockSpace[j][i] = block[j][i];
	}

	else if (nextBlock == squareType)
	{
		int block[5][5] =
		{
			0, 0, 1, 1, 0,
			0, 0, 1, 1, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0
		};

		//copy block type
		for (int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; ++j)
				nextBlockSpace[j][i] = block[j][i];
	}

	else if (nextBlock == lType)
	{
		int block[5][5] =
		{
			0, 0, 0, 0, 0,
			0, 1, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0
		};

		//copy block type
		for (int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; ++j)
				nextBlockSpace[j][i] = block[j][i];
	}

	else if (nextBlock == bLType)
	{
		int block[5][5] =
		{
			0, 0, 0, 0, 0,
			0, 1, 1, 0, 0,
			0, 1, 0, 0, 0,
			0, 1, 0, 0, 0
		};

		//copy block type
		for (int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; ++j)
				nextBlockSpace[j][i] = block[j][i];
	}

	else if (nextBlock == tType)
	{
		int block[5][5] =
		{
			0, 0, 0, 0, 0,
			0, 1, 1, 1, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, 0, 0
		};

		//copy block type
		for (int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; ++j)
				nextBlockSpace[j][i] = block[j][i];
	}

	else if (nextBlock == angledType)
	{
		int block[5][5] =
		{
			0, 1, 0, 0, 0,
			0, 1, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, 0, 0
		};

		//copy block type
		for (int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; ++j)
				nextBlockSpace[j][i] = block[j][i];
	}

	window.draw(box);
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
		{
			if (nextBlockSpace[j][i] == 1)
			{
				sf::RectangleShape blockPiece;
				blockPiece.setFillColor(sf::Color::Red);
				blockPiece.setSize(sf::Vector2f(32, 32));
				blockPiece.setPosition(32.f * i + 721, 32.f * j + 151);
				blockPiece.setOutlineColor(sf::Color::White);
				blockPiece.setOutlineThickness(1);
				window.draw(blockPiece);
			}
		}
}

void Game::update()
{
	int lastGameState = gameState;
	int currentHighlighted = 0;
	int startingLevel = 0;
	bool keyEnable = true;
	sf::RectangleShape levelDisplay;

	while (window.isOpen())
	{
		if (lastGameState != gameState)
		{
			music.stop();
			playSpace.reset();
		}
		lastGameState = gameState;
		music.play(gameState, playSpace.almostFull(music));

		if (gameState == 0)
		{
			std::string titleStr = "Tetris";
			std::string startStr = "Start Game";
			std::string levelStr = "Level";
			std::string quitStr = "Quit";
			std::string diffStr = "DIFFICULTY";

			sf::Text titleText;
			sf::Text startText;
			sf::Text levelText;
			sf::Text quitText;
			sf::Text diffText;
			sf::RectangleShape levelGauge;
			levelGauge.setPosition(sf::Vector2f(480 - startingLevel * 5.f, 600));
			levelGauge.setOutlineColor(sf::Color::White);
			levelGauge.setOutlineThickness(1);
			levelGauge.setSize(sf::Vector2f(10 + startingLevel * 10.f, 10.f));
			levelGauge.setFillColor(sf::Color(10 * startingLevel, 0, 200 - startingLevel * 5, 255));

			titleText.setString(titleStr);
			titleText.setCharacterSize(120);
			titleText.setFont(font);
			titleText.setColor(sf::Color::Green);
			titleText.setPosition(340, 40);

			startText.setString(startStr);
			startText.setCharacterSize(70);
			startText.setFont(font);
			startText.setColor(sf::Color::Red);
			startText.setPosition(340, 200);

			levelText.setString(levelStr);
			levelText.setCharacterSize(70);
			levelText.setFont(font);
			levelText.setColor(sf::Color::Red);
			levelText.setPosition(400, 300);

			quitText.setString(quitStr);
			quitText.setCharacterSize(70);
			quitText.setFont(font);
			quitText.setColor(sf::Color::Red);
			quitText.setPosition(420, 400);

			diffText.setString(diffStr);
			diffText.setCharacterSize(70);
			diffText.setFont(font);
			diffText.setColor(sf::Color(10 * startingLevel, 0, 200 - startingLevel * 5, 255));
			diffText.setPosition(290, 650);

			switch (currentHighlighted)
			{
			case 0:
				startText.setColor(sf::Color::Magenta);
				break;

			case 1:
				levelText.setColor(sf::Color::Magenta);
				break;

			case 2:
				quitText.setColor(sf::Color::Magenta);
				break;
			}

			while(window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.key.code == sf::Keyboard::Return)
				{
					if (currentHighlighted == 0)
					{
						gameState = 1;
						score = 0;
						blocksBroken = 0;
						win = false;
					}

					else if (currentHighlighted == 2)
						window.close();
				}

				else if (currentHighlighted == 1)
				{
					if (event.type == sf::Event::KeyPressed)
					{
						if (event.key.code == sf::Keyboard::Right)
						{
							++startingLevel;
							if (startingLevel > 25)
								startingLevel = 25;
						}

						else if (event.key.code == sf::Keyboard::Left)
						{
							--startingLevel;
							if (startingLevel < 0)
								startingLevel = 0;
						}
					}

					//sets in game level to selection
					playSpace.setLevel(startingLevel + 1);
					level = startingLevel;
				}

				if (keyEnable)
				{
					if (event.key.code == sf::Keyboard::Up)
					{
						if (currentHighlighted > 0)
							--currentHighlighted;

						keyEnable = false;
					}

					else if (event.key.code == sf::Keyboard::Down)
					{
						if (currentHighlighted < 2)
							++currentHighlighted;

						keyEnable = false;
					}
				}

				if (event.type == sf::Event::KeyReleased)
				{
					if (event.key.code == sf::Keyboard::Up)
						keyEnable = true;

					else if (event.key.code == sf::Keyboard::Down)
						keyEnable = true;
				}
			}

			window.clear();
			window.draw(background);
			window.draw(titleText);
			window.draw(startText);
			window.draw(levelText);
			window.draw(quitText);
			window.draw(levelGauge);
			window.draw(diffText);
			window.display();
		}

		else if (gameState == 1)
		{
			direction = none;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Right)
						direction = right;

					else if (event.key.code == sf::Keyboard::Down)
						direction = down;

					else if (event.key.code == sf::Keyboard::Left)
						direction = left;

					else if (event.key.code == sf::Keyboard::Up)
						direction = up;

					else if (event.key.code == sf::Keyboard::Space)
						direction = skipDown;

					else if (event.key.code == sf::Keyboard::Escape)
						gameState = 0;
				}
			}

			calculateScore(); //calls updates to playspace

			window.clear();
			window.draw(background);
			playSpace.drawPlaySpace(window);
			showScore();
			showLevel();
			showNextBlock();
			window.display();
		}

		else if (gameState == 777) //win
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				else if (event.type == sf::Event::KeyPressed)
					if (event.key.code == sf::Keyboard::Escape)
						gameState = 0;
			}

			window.clear(sf::Color(32, 56, 236, 255));
			winSprite.setScale(sf::Vector2f(3.8f, 3.8f));
			winSprite.setPosition(sf::Vector2f(15, -50));
			window.draw(winSprite);

			//score
			std::string tmp;
			std::stringstream ss;
			ss << score;	
			ss >> tmp;
			sf::Text infoText;
			infoText.setString(tmp);
			infoText.setCharacterSize(50);
			infoText.setFont(nintendoFont);
			infoText.setColor(sf::Color::White);
			infoText.setPosition(430, 15);
			window.draw(infoText);

			//level
			tmp.clear();
			ss.clear();
			ss << level;
			ss >> tmp;
			infoText.setString(tmp);
			infoText.setPosition(430, 55);
			window.draw(infoText);

			//height
			tmp.clear();
			ss.clear();
			ss << blocksBroken;
			ss >> tmp;
			infoText.setString(tmp);
			infoText.setPosition(430, 95);
			window.draw(infoText);

			window.display();
		}
	}
}