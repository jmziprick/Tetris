#pragma once

class AngledBlock
{
public:
	AngledBlock();
	const char* getBlockArray();
	int getStart();

private:
	int currentRotation;

	enum rotationDirection{ up, right, down, left };
};