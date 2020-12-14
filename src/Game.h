#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <sstream>
#include "PlaySpace.h"
#include "Music.h"
#include "Sound.h"

class Game
{
public:
	Game();
	~Game();

	void init();
	void update();
	

private:
	void updateLevel();
	void showScore();
	void showLevel();
	void showNextBlock();
	void calculateScore();

	sf::RenderWindow window;
	sf::Event event;

	sf::Font font;
	sf::Font nintendoFont;

	sf::Texture backgroundText;
	sf::Sprite background;

	sf::Texture winText;
	sf::Sprite winSprite;

	int score;
	int direction;
	int level;
	int blocksBroken;
	int remainingBlocks;

	int gameState;

	bool win;

	PlaySpace playSpace;
	Music music;
	Sound sound;
};