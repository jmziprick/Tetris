#pragma once
#include <SFML/Audio.hpp>

class Music
{
public:
	Music();
	void play(int gameState, bool fast);
	void stop();
	
private:
	sf::Music music;
	sf::Time musicOffset;
	bool playing;
};