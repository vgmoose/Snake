#ifndef SNAKE_H
#define SNAKE_H
#include "draw.h"
#include "system/memory.h"
#include <vpad/input.h>

typedef struct SnakeI SnakeI;
struct SnakeI {
	// Coordinates
    int x;
    int y;
    int length;
    char direction;
    // Colors
    int r;
    int g;
    int b;

    int end;

    SnakeI *next;
};

typedef struct Snake Snake;
struct Snake {
	int frame_x;
	int frame_y;
	int x;
	int y;
	int w;	
	int length;
	char direction;
	int loose;
    int pause;
	int score;
    int start;
	int food_state;
	int food_x;
	int food_y;
    int seed;
	int debug;

	SnakeI *first;
};

void initSnake(Snake* s);
void displayStart(Snake* s);
void displaySnake(Snake* s);
void moveSnake(Snake* s, char direction);
int isOnSnake(Snake* s, int head, int x, int y);
int isLooseMoove(Snake* s);
void setNewFoodCoord(Snake* s);
void triggerSnake(Snake* s, VPADStatus* vpad);
void stretchSnake(Snake* s, int length);
int randU(int range_max, Snake* s);


#endif /* SNAKE_H */
