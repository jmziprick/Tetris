#pragma once

class BackwardsLBlock
{
public:
	BackwardsLBlock();
	const char* getBlockArray();
	int getStart();

private:
	int currentRotation;

	enum rotationDirection{ up, right, down, left };
};