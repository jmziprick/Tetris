#pragma once

class TBlock
{
public:
	TBlock();
	const char* getBlockArray();
	int getStart();

private:
	int currentRotation;

	enum rotationDirection{ up, right, down, left };
};