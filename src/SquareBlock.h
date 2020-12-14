#pragma once
#include <cstdlib>

class SquareBlock
{
public:
	SquareBlock();
	const char* getBlockArray();

	int getStart();

private:
	int currentRotation;

	enum rotationDirection{ up, right, down, left };
};