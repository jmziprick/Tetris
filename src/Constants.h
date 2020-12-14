#ifndef CONSTANTS_H
#define CONSTANTS_H

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

const float BOARD_POS_X = 350;
const float BOARD_POS_Y = 100;

const int MOVE_BLOCK_SPEED = 1000;
const int FLOAT_TIME_BASE = 800;

const int FPS = 60;

const int BLOCKS_TILL_NEXT_LEVEL = 10;
const int TOTAL_BLOCK_TYPES = 6;

const int WIN_SCORE = 5000;

enum directions{ none, right, down, left, up, skipDown};
enum blockTypes{ longType, squareType, lType, bLType, tType, angledType};
enum soundType{landSound, breakSound, tetrisSound, rotateSound, levelUpSound, moveSound, loseSound, gameOverSound};

#endif