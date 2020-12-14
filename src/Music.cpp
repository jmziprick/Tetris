#include "Music.h"


Music::Music()
{
	music.setVolume(30);
	music.openFromFile("tetris a.ogg");
	music.setLoop(true);
	playing = false;
}

void Music::play(int gameState, bool fast)
{
	if (playing)
	{
		if (gameState == 0)
			musicOffset = sf::Time::Zero;

		else if (gameState == 1)
			musicOffset = music.getPlayingOffset();

		else if (gameState == 777)
		{
			fast = false;
			musicOffset = sf::Time::Zero;
		}
	}

	else
	{
		if (gameState == 0)
			music.openFromFile("tetris menu.ogg");

		else if (gameState == 1)
		{
			if (fast == false)
			{
				music.openFromFile("tetris a.ogg");
				music.setPlayingOffset(musicOffset);
			}

			else if (fast == true)
			{
				music.openFromFile("tetris a(fast).ogg");
				music.setPlayingOffset(musicOffset);
			}
		}

		else if (gameState == 777)
		{
			music.openFromFile("fanfare.ogg");
		}
		
		playing = true;
		music.play();
	}
}

void Music::stop()
{
	playing = false;
	music.stop();
}