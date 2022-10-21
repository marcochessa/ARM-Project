#include "LPC17xx.h"

typedef struct{
	int x, y;
	uint8_t speed;
	uint8_t dimension;
	uint16_t direction;
	int move_x, move_y;
} ball_t;


typedef struct {
	uint8_t width;
	uint8_t height;
	uint8_t padding;
	int x;
	uint16_t score;
} paddle_t;




void start_game(uint8_t balldim, uint8_t walldep, uint8_t paddlew, uint8_t paddleh, uint8_t padding);
void reset_game(void);
void newBall(void);


void checkWall(void);
uint8_t checkPaddle(void);
void checkGameOver(void);

void sound(int freq);

void point(uint8_t k);

void looser(uint8_t k);

