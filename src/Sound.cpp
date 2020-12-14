#include "Sound.h"


Sound::Sound()
{
	landSoundBuff.loadFromFile("land.ogg");
	breakSoundBuff.loadFromFile("break.ogg");
	tetrisSoundBuff.loadFromFile("tetris.ogg");
	rotateSoundBuff.loadFromFile("rotate.ogg");
	levelUpSoundBuff.loadFromFile("lvl up.ogg");
	moveSoundBuff.loadFromFile("move.ogg");
	loseSoundBuff.loadFromFile("lose.ogg");
	gameOverSoundBuff.loadFromFile("game over.ogg");

	landSound.setBuffer(landSoundBuff);
	breakSound.setBuffer(breakSoundBuff);
	rotateSound.setBuffer(rotateSoundBuff);
	tetrisSound.setBuffer(tetrisSoundBuff);
	levelUpSound.setBuffer(levelUpSoundBuff);
	moveSound.setBuffer(moveSoundBuff);
	loseSound.setBuffer(loseSoundBuff);
	gameOverSound.setBuffer(gameOverSoundBuff);
}

void Sound::playSound(int soundNum)
{
	switch (soundNum)
	{
	case soundType::breakSound:
		breakSound.play();
		break;

	case soundType::landSound:
		landSound.play();
		break;

	case soundType::rotateSound:
		rotateSound.play();
		break;

	case soundType::tetrisSound:
		tetrisSound.play();
		break;

	case soundType::levelUpSound:
		levelUpSound.play();
		break;

	case soundType::moveSound:
		moveSound.play();
		break;

	case soundType::loseSound:
		loseSound.play();
		break;

	case soundType::gameOverSound:
		gameOverSound.play();
		break;
	}
}