#pragma once

class LBlock
{
public:
	LBlock();
	const char* getBlockArray();
	int getStart();

private:
	int currentRotation;

	enum rotationDirection{ up, right, down, left };
};

