#pragma once
#include <SFML/Audio.hpp>
#include "Constants.h"

class Sound
{
public:
	Sound();
	void playSound(int soundNum);

private:
	sf::Sound landSound;
	sf::Sound breakSound;
	sf::Sound tetrisSound;
	sf::Sound rotateSound;
	sf::Sound levelUpSound;
	sf::Sound moveSound;
	sf::Sound loseSound;
	sf::Sound gameOverSound;

	sf::SoundBuffer landSoundBuff;
	sf::SoundBuffer breakSoundBuff;
	sf::SoundBuffer tetrisSoundBuff;
	sf::SoundBuffer rotateSoundBuff;
	sf::SoundBuffer levelUpSoundBuff;
	sf::SoundBuffer moveSoundBuff;
	sf::SoundBuffer loseSoundBuff;
	sf::SoundBuffer gameOverSoundBuff;
};