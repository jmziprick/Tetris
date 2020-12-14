#include "BackwardsLBlock.h"

BackwardsLBlock::BackwardsLBlock()
{
	currentRotation = up;
}

const char* BackwardsLBlock::getBlockArray()
{
	const static char blockUp[] =
	{
		0, 0, 0, 2, 0,
		0, 0, 0, 2, 0,
		0, 0, 2, 2, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};

	const static char blockRight[] =
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 2, 2, 2, 2,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};

	switch (currentRotation)
	{
	case up:
		return blockUp;
		break;

	case right:
		return blockRight;
		break;

	case down:
		return blockUp;
		break;

	case left:
		return blockRight;
		break;

	default:
		return blockUp;
		break;
	}
	return 0;
}

int BackwardsLBlock::getStart()
{
	if (currentRotation == right || currentRotation == left)
		return -2;

	return 0;
}