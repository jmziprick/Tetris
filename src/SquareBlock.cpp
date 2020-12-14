#include "SquareBlock.h"

SquareBlock::SquareBlock()
{
	currentRotation = up;
}

const char* SquareBlock::getBlockArray()
{
	const static char blockUp[] =
	{
		0, 0, 2, 2, 0,
		0, 0, 2, 2, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};

	switch (currentRotation)
	{
	case up:
		return blockUp;
		break;

	case right:
		return blockUp;
		break;

	case down:
		return blockUp;
		break;

	case left:
		return blockUp;
		break;

	default:
		return blockUp;
		break;
	}
	return 0;
}

int SquareBlock::getStart()
{
	return 0;
}