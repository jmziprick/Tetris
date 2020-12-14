#pragma once
#include <cstdlib>

class LongBlock
{
public:
	LongBlock();
	const char* getBlockArray();

	int getStart();

private:
	int currentRotation;

	enum rotationDirection{ up, right, down, left };
};